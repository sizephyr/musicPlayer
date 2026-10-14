#include "filemanagebackend.h"

#include <QBuffer>
#include <QCoroProcess>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QLoggingCategory>
#include <QProcess>
#include <QStandardPaths>
#include <QUrl>

namespace {
const auto loggingCategory = QLoggingCategory("file_manage_backend");

// Оставляем ТОЛЬКО для заголовка ID3v2
auto parseSynchsafeInt = [](const char* bytes) -> int {
  return ((bytes[0] & 0x7F) << 21) | ((bytes[1] & 0x7F) << 14) |
         ((bytes[2] & 0x7F) << 7) | (bytes[3] & 0x7F);
};

// Добавляем для размеров фреймов (ID3v2.3)
auto parseNormalInt = [](const char* bytes) -> int {
  return (static_cast<unsigned char>(bytes[0]) << 24) |
         (static_cast<unsigned char>(bytes[1]) << 16) |
         (static_cast<unsigned char>(bytes[2]) << 8) |
         static_cast<unsigned char>(bytes[3]);
};
}  // namespace

std::expected<QList<std::filesystem::path>, QString>
FileManageBackend::filesSearch(const QString& path) {
  const std::filesystem::path targetPath =
      QUrl(path).toLocalFile().toStdString();

  qCInfo(loggingCategory) << "Target path:" << targetPath;

  std::error_code ec;
  if (!std::filesystem::exists(targetPath, ec)) {
    return std::unexpected(QStringLiteral("Path does not exist: ") +
                           QString::fromStdString(targetPath.string()));
  }

  QList<std::filesystem::path> result;

  if (std::filesystem::is_regular_file(targetPath, ec)) {
    if (targetPath.extension() == ".mp3") {
      result.append(targetPath);
      return result;
    }
    return std::unexpected(QStringLiteral("Provided file is not an MP3 file."));
  }

  if (std::filesystem::is_directory(targetPath, ec)) {
    auto mp3FilesView =
        std::filesystem::directory_iterator(
            targetPath,
            std::filesystem::directory_options::skip_permission_denied) |
        std::views::filter(
            [](const auto& entry) { return entry.is_regular_file(); }) |
        std::views::filter([](const auto& entry) {
          return entry.path().extension() == ".mp3";
        }) |
        std::views::transform([](const auto& entry) { return entry.path(); });

    std::ranges::copy(mp3FilesView, std::back_inserter(result));

    if (result.isEmpty()) {
      return std::unexpected(
          QStringLiteral("No MP3 files found in directory."));
    }

    return result;
  }

  return std::unexpected(QStringLiteral("Unsupported file system object."));
}

std::shared_ptr<Common::Mp3Description> FileManageBackend::parseMp3(
    const std::filesystem::path& filePath) {
  return parseID3(filePath);
}

QString decodeText(const QByteArray& data) {
  if (data.isEmpty()) {
    return QString();
  }

  char encoding = data[0];
  QByteArray textData = data.mid(1);
  QString result;

  if (encoding == 0) {
    result = QString::fromLatin1(textData);
  } else if (encoding == 1) {
    result = QString::fromUtf16(
        reinterpret_cast<const char16_t*>(textData.constData() + 2),
        (textData.size() - 2) / 2);
  } else if (encoding == 2) {
    result = QString::fromUtf16(
        reinterpret_cast<const char16_t*>(textData.constData()),
        textData.size() / 2);
  } else if (encoding == 3) {
    result = QString::fromUtf8(textData);
  } else {
    result = QString::fromLatin1(textData);
  }

  while (!result.isEmpty() && result.endsWith(QChar('\0'))) {
    result.chop(1);
  }

  return result.trimmed();
}

std::shared_ptr<Common::Mp3Description> FileManageBackend::parseID3(
    const std::filesystem::path& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qCWarning(loggingCategory) << "Cannot open file for reading:" << filePath;
    return nullptr;
  }

  QByteArray header = file.read(10);
  if (header.size() < 10 || !header.startsWith("ID3")) {
    qCWarning(loggingCategory) << "No ID3 tag found in file:" << filePath;
    return nullptr;
  }

  const int tagSize = parseSynchsafeInt(header.constData() + 6);
  const QByteArray tagData = file.read(tagSize);

  int pos = 0;
  std::optional<std::filesystem::path> coverImagePath;
  std::optional<Common::Mp3DescriptionTypes::TrackTitle> trackTitle;
  std::optional<Common::Mp3DescriptionTypes::TrackArtist> TrackArtist;
  while (pos < tagSize) {
    if (pos + 10 > tagData.size()) {
      // qCWarning(loggingCategory)
      //     << "Incomplete frame header at position:" << pos;
      break;
    }

    const char* frameHeaderPtr = tagData.constData() + pos;
    const int frameSize = parseNormalInt(frameHeaderPtr + 4);

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
      /// \todo попытаться нормально изымать обложки
      coverImagePath = getCoverImagePath(frameData);
    }

    pos += 10 + frameSize;
  }

  if (trackTitle.has_value() && TrackArtist.has_value()) {
    return std::make_shared<Common::Mp3Description>(
        filePath, trackTitle.value(), TrackArtist.value());
  }
  return nullptr;
}

std::optional<std::filesystem::path> FileManageBackend::getCoverImagePath(
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

  return std::filesystem::path(tempPath.toStdString());
}

QCoro::Task<std::expected<void, QString>>
FileManageBackend::fillCoverImagePathByFfmpegCLI(
    std::shared_ptr<const Common::Mp3Description> descr,
    std::filesystem::path targetImagePath) {
  QProcess process;

  // Настраиваем исполняемый файл и аргументы команды
  process.setProgram(QStringLiteral("ffmpeg"));
  process.setArguments({
      QStringLiteral("-y"),  // Принудительная перезапись без вопросов!
      QStringLiteral("-i"),
      descr->path().c_str(),
      QStringLiteral("-map"),
      QStringLiteral("0:v:0"),
      QStringLiteral("-c:v"),
      QStringLiteral("copy"),
      QStringLiteral("-update"),
      QStringLiteral("1"),
      targetImagePath.c_str(),
  });

  process.start();

  if (!co_await qCoro(process).waitForStarted()) {
    co_return std::unexpected("Не удалось запустить процесс ffmpeg!");
  }

  co_await qCoro(process).waitForFinished();

  // Проверяем результаты выполнения
  if (process.exitStatus() == QProcess::NormalExit && process.exitCode() != 0) {
    co_return std::unexpected("Ошибка выполнения ffmpeg: " +
                              process.readAllStandardError());
  }

  co_return {};
}
