#include "filemanagebackend.h"

#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QLoggingCategory>
#include <QUrl>

namespace {
const auto loggingCategory = QLoggingCategory("file_manage_backend");

auto parseSynchsafeInt = [](const char* bytes) -> int {
  return ((bytes[0] & 0x7F) << 21) | ((bytes[1] & 0x7F) << 14) |
         ((bytes[2] & 0x7F) << 7) | (bytes[3] & 0x7F);
};
}  // namespace

std::expected<QList<std::filesystem::path>, QString>
FileManageBackend::filesSearch(const QString& path) {
  const std::filesystem::path directoryPath =
      QUrl(path).toLocalFile().toStdString();

  qCDebug(loggingCategory) << "Original path:" << path;
  qCDebug(loggingCategory) << "Converted directory path:" << directoryPath;

  const QDir directory(directoryPath);
  if (!directory.exists()) {
    return std::unexpected(QStringLiteral("Directory does not exist: ") +
                           directoryPath.c_str());
  }

  const QStringList entries =
      directory.entryList({QStringLiteral("*.mp3")}, QDir::Files);

  qCDebug(loggingCategory) << "Files found:" << entries;

  if (entries.isEmpty()) {
    return std::unexpected(QStringLiteral("No MP3 files found in directory."));
  }

  QList<std::filesystem::path> result;
  result.reserve(entries.size());
  std::ranges::transform(entries, std::back_inserter(result),
                         [directoryPath](const auto& entry) {
                           return directoryPath / entry.toStdString();
                         });

  return result;
}

std::optional<Common::Mp3Description> FileManageBackend::parseMp3(
    const std::filesystem::path& filePath) {
  return parseID3(filePath);
}

QString decodeText(const QByteArray& data) {
  if (data.isEmpty()) {
    return QString();
  }

  // Check encoding byte
  char encoding = data[0];
  QByteArray textData = data.mid(1);

  if (encoding == 0) {
    // ISO-8859-1
    return QString::fromLatin1(textData);
  } else if (encoding == 1) {
    // UTF-16 with BOM
    return QString::fromUtf16(
        reinterpret_cast<const char16_t*>(textData.constData() + 2),
        (textData.size() - 2) / 2);
  } else if (encoding == 2) {
    // UTF-16BE without BOM
    return QString::fromUtf16(
        reinterpret_cast<const char16_t*>(textData.constData()),
        textData.size() / 2);
  } else if (encoding == 3) {
    // UTF-8
    return QString::fromUtf8(textData);
  }

  // Default to ISO-8859-1 if encoding is unknown
  return QString::fromLatin1(textData);
}

std::optional<Common::Mp3Description> FileManageBackend::parseID3(
    const std::filesystem::path& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qCWarning(loggingCategory) << "Cannot open file for reading:" << filePath;
    return std::nullopt;
  }

  QByteArray header = file.read(10);
  if (header.size() < 10 || !header.startsWith("ID3")) {
    qCWarning(loggingCategory) << "No ID3 tag found in file:" << filePath;
    return std::nullopt;
  }

  const int tagSize = parseSynchsafeInt(header.constData() + 6);
  const QByteArray tagData = file.read(tagSize);

  int pos = 0;
  std::optional<QUrl> coverImagePath;
  std::optional<Common::Mp3DescriptionTypes::TrackTitle> trackTitle;
  std::optional<Common::Mp3DescriptionTypes::TrackArtist> TrackArtist;
  while (pos < tagSize) {
    if (pos + 10 > tagData.size()) {
      qCWarning(loggingCategory)
          << "Incomplete frame header at position:" << pos;
      break;
    }

    const char* frameHeaderPtr = tagData.constData() + pos;
    const int frameSize = parseSynchsafeInt(frameHeaderPtr + 4);

    if (pos + 10 + frameSize > tagData.size()) {
      qCWarning(loggingCategory) << "Incomplete frame data at position:" << pos;
      break;
    }

    // C++23/Qt 6: используем легковесный QByteArrayView вместо выделения памяти
    // через mid()
    const QByteArrayView frameID(frameHeaderPtr, 4);
    const QByteArrayView frameData(frameHeaderPtr + 10, frameSize);

    if (frameID == "TIT2") {
      trackTitle = Common::Mp3DescriptionTypes::TrackTitle(
          decodeText(frameData.toByteArray()));
    } else if (frameID == "TPE1") {
      TrackArtist = Common::Mp3DescriptionTypes::TrackArtist(
          decodeText(frameData.toByteArray()));
    } else if (frameID == "APIC") {
      coverImagePath = getCoverImagePath(frameData);
    }

    pos += 10 + frameSize;
  }

  if (trackTitle.has_value() && TrackArtist.has_value()) {
    return Common::Mp3Description(trackTitle.value(), TrackArtist.value(),
                                  coverImagePath);
  }
  return std::nullopt;
}

std::optional<QUrl> FileManageBackend::getCoverImagePath(
    QByteArrayView frameData) {
  // В APIC-фрейме сначала идет Text Encoding (1 байт)
  const int mimeTypeEnd = frameData.indexOf('\0', 1);
  if (mimeTypeEnd == -1) {
    return std::nullopt;
  }

  const QString mimeType =
      QString::fromLatin1(frameData.slice(1, mimeTypeEnd - 1));

  // Пропускаем MIME-тип и '\0', ищем конец описания картинки (Description)
  const int descriptionEnd = frameData.indexOf('\0', mimeTypeEnd + 1);
  if (descriptionEnd == -1) {
    return std::nullopt;
  }

  const auto imageData = frameData.sliced(descriptionEnd + 1);
  const char* format = nullptr;

  if (mimeType.contains(QStringLiteral("jpeg")))
    format = "JPEG";
  else if (mimeType.contains(QStringLiteral("png")))
    format = "PNG";
  else {
    qCWarning(loggingCategory)
        << "Unsupported image format in APIC frame:" << mimeType;
    return std::nullopt;
  }

  QImage coverImage;
  if (!coverImage.loadFromData(reinterpret_cast<const uchar*>(imageData.data()),
                               imageData.size(), format)) {
    qCWarning(loggingCategory) << "Failed to load image from APIC frame";
    return std::nullopt;
  }

  const QString tempPath = QDir::tempPath() + QStringLiteral("/coverImage.jpg");
  if (!coverImage.save(tempPath, "JPEG")) {
    qCWarning(loggingCategory)
        << "Failed to save cover image to temporary path";
    return std::nullopt;
  }

  return QUrl::fromLocalFile(tempPath);
}
