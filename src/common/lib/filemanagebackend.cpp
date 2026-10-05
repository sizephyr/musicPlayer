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

FileManageBackend::FileManageBackend(QObject* parent) : QObject{parent} {}

QStringList FileManageBackend::fileSearch(const QString& path) {
  const QString directoryPath = QUrl(path).toLocalFile();

  qCDebug(loggingCategory) << "Original path:" << path;
  qCDebug(loggingCategory) << "Converted directory path:" << directoryPath;

  const QDir directory(directoryPath);
  if (!directory.exists()) {
    qCWarning(loggingCategory) << "Directory does not exist:" << directoryPath;
    return {};
  }

  const QStringList entries =
      directory.entryList({QStringLiteral("*.mp3")}, QDir::Files);

  qCDebug(loggingCategory) << "Files found:" << entries;

  if (entries.isEmpty()) {
    qCWarning(loggingCategory) << "No MP3 files found in directory.";
    return {};
  }

  QStringList result;
  result.reserve(entries.size());

  for (const QString& entry : entries) {
    result.append(entry);
    qCDebug(loggingCategory) << "Added file name:" << entry;
  }

  return result;
}

void FileManageBackend::parseMp3(const QString& filePath) {
  parseID3(filePath);
  emit titleChanged();
  emit artistChanged();
  emit coverImagePathChanged();
}

QString FileManageBackend::title() const { return _title; }

QString FileManageBackend::artist() const { return _artist; }

QString FileManageBackend::coverImagePath() const { return _coverImagePath; }

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

void FileManageBackend::parseID3(const QString& filePath) {
  _coverImagePath.clear();

  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qCWarning(loggingCategory) << "Cannot open file for reading:" << filePath;
    return;
  }

  QByteArray header = file.read(10);
  if (header.size() < 10 || !header.startsWith("ID3")) {
    qCWarning(loggingCategory) << "No ID3 tag found in file:" << filePath;
    return;
  }

  const int tagSize = parseSynchsafeInt(header.constData() + 6);
  const QByteArray tagData = file.read(tagSize);

  int pos = 0;
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
      _title = decodeText(frameData.toByteArray());
    } else if (frameID == "TPE1") {
      _artist = decodeText(frameData.toByteArray());
    } else if (frameID == "APIC") {
      processApicFrame(frameData);
    }

    pos += 10 + frameSize;
  }
}

void FileManageBackend::processApicFrame(QByteArrayView frameData) {
  // В APIC-фрейме сначала идет Text Encoding (1 байт)
  const int mimeTypeEnd = frameData.indexOf('\0', 1);
  if (mimeTypeEnd == -1) return;

  const QString mimeType =
      QString::fromLatin1(frameData.slice(1, mimeTypeEnd - 1));

  // Пропускаем MIME-тип и '\0', ищем конец описания картинки (Description)
  const int descriptionEnd = frameData.indexOf('\0', mimeTypeEnd + 1);
  if (descriptionEnd == -1) return;

  const auto imageData = frameData.sliced(descriptionEnd + 1);
  const char* format = nullptr;

  if (mimeType.contains(QStringLiteral("jpeg")))
    format = "JPEG";
  else if (mimeType.contains(QStringLiteral("png")))
    format = "PNG";
  else {
    qCWarning(loggingCategory)
        << "Unsupported image format in APIC frame:" << mimeType;
    return;
  }

  QImage coverImage;
  if (!coverImage.loadFromData(reinterpret_cast<const uchar*>(imageData.data()),
                               imageData.size(), format)) {
    qCWarning(loggingCategory) << "Failed to load image from APIC frame";
    return;
  }

  const QString tempPath = QDir::tempPath() + QStringLiteral("/coverImage.jpg");
  if (!coverImage.save(tempPath, "JPEG")) {
    qCWarning(loggingCategory)
        << "Failed to save cover image to temporary path";
    return;
  }

  _coverImagePath = QStringLiteral("file://") + tempPath;
}
