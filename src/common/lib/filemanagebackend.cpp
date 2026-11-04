#include "filemanagebackend.h"

#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>

#include <QCoroProcess>
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

auto isMp3 = [](const auto& entry) {
  return entry.is_regular_file() && entry.path().extension() == ".mp3";
};
}  // namespace

std::expected<QList<std::filesystem::path>, QString>
FileManageBackend::filesSearch(const QString& path) {
  const std::filesystem::path targetPath =
      QUrl(path).toLocalFile().toStdString();
  const auto options =
      std::filesystem::directory_options::skip_permission_denied;

  qCInfo(loggingCategory) << "Поиск mp3 файлов по пути" << targetPath;

  if (!std::filesystem::exists(targetPath)) {
    return std::unexpected(QStringLiteral("Путь недоступен: ") + path);
  }

  if (!std::filesystem::is_directory(targetPath)) {
    return std::unexpected(
        QStringLiteral("Переданный путь не является директорией."));
  }

  auto recursiveFiles =
      std::filesystem::recursive_directory_iterator(targetPath, options) |
      std::views::filter(isMp3) |
      std::views::transform([](const auto& entry) { return entry.path(); });

  QList<std::filesystem::path> result;
  std::ranges::copy(recursiveFiles, std::back_inserter(result));

  if (result.isEmpty()) {
    return std::unexpected(QStringLiteral("Не найдены MP3-файлы."));
  }

  return result;
}

std::expected<QList<std::filesystem::path>, QString>
FileManageBackend::collectMp3(const QString& path) {
  const std::filesystem::path targetPath =
      QUrl(path).toLocalFile().toStdString();

  qCInfo(loggingCategory) << "Поиск mp3 файлов по пути" << targetPath;

  std::error_code ec;
  if (!std::filesystem::exists(targetPath, ec)) {
    return std::unexpected(QStringLiteral("Путь недоступен: ") +
                           QString::fromStdString(targetPath.string()));
  }

  if (std::filesystem::is_regular_file(targetPath, ec)) {
    if (targetPath.extension() == ".mp3") {
      return QList<std::filesystem::path>{targetPath};
    }
    return std::unexpected(
        QStringLiteral("Предоставленный файл не является mp3 файлом."));
  }

  return std::unexpected(QStringLiteral("Переданный путь не является файлом."));
}

std::shared_ptr<Common::Mp3Description> FileManageBackend::parseMp3(
    const std::filesystem::path& filePath) {
  return parseID3(filePath);
}

std::shared_ptr<Common::Mp3Description> FileManageBackend::parseID3(
    const std::filesystem::path& filePath) {
  using namespace Common::Mp3DescriptionTypes;

  TagLib::FileRef f(filePath.c_str());
  if (f.isNull() || f.tag() == nullptr) {
    qCWarning(loggingCategory) << "Не удалось получить теги файла" << filePath;
    return nullptr;
  }

  if (f.tag()->title().isEmpty()) {
    qCWarning(loggingCategory) << "Тег названия пуст";
    return nullptr;
  }
  if (f.tag()->artist().isEmpty()) {
    qCWarning(loggingCategory) << "Тег исполнителя пуст";
    return nullptr;
  }

  return std::make_shared<Common::Mp3Description>(
      filePath, TrackTitle(f.tag()->title().to8Bit(true)),
      TrackArtist(f.tag()->artist().to8Bit(true)));
}

QCoro::Task<std::expected<void, QString>>
FileManageBackend::fillCoverImagePathByFfmpegCLI(
    std::shared_ptr<const Common::Mp3Description> descr,
    std::filesystem::path targetImagePath) {
  QProcess process;
  QString ffmpegPath = QStandardPaths::findExecutable(QStringLiteral("ffmpeg"));

#ifdef Q_OS_MAC
  if (ffmpegPath.isEmpty()) {
    ffmpegPath = QStandardPaths::findExecutable(
        QStringLiteral("ffmpeg"), {QStringLiteral("/opt/homebrew/bin"),
                                   QStringLiteral("/usr/local/bin")});
  }
#endif

  if (ffmpegPath.isEmpty()) {
    co_return std::unexpected("Исполняемый файл ffmpeg не найден в системе!");
  }

  process.setProgram(ffmpegPath);
  process.setArguments({
      QStringLiteral("-y"),
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

  if (process.exitStatus() == QProcess::NormalExit && process.exitCode() != 0) {
    co_return std::unexpected("Ошибка выполнения ffmpeg: " +
                              process.readAllStandardError());
  }

  co_return {};
}

std::expected<void, QString> FileManageBackend::fillCoverImagePathByTagLib(
    std::shared_ptr<const Common::Mp3Description> descr,
    std::filesystem::path targetImagePath) {
  TagLib::MPEG::File mpegFile(descr->path().c_str());
  if (!mpegFile.isValid() || !mpegFile.ID3v2Tag()) {
    return std::unexpected(
        QStringLiteral("Невалидный MP3 файл или отсутствует ID3v2 тег."));
  }

  auto* tag = mpegFile.ID3v2Tag();
  const auto& frameList = tag->frameListMap()["APIC"];
  if (frameList.isEmpty()) {
    return std::unexpected(
        QStringLiteral("В файле не найдена обложка (APIC фрейм)."));
  }

  auto* pictureFrame =
      static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
  if (!pictureFrame) {
    return std::unexpected(QStringLiteral("Не удалось прочитать APIC фрейм."));
  }

  const auto& pictureData = pictureFrame->picture();
  QImage coverImage;
  if (!coverImage.loadFromData(
          reinterpret_cast<const uchar*>(pictureData.data()),
          pictureData.size())) {
    return std::unexpected(
        QStringLiteral("Не удалось загрузить изображение из тега."));
  }

  const QString savePath = QString::fromStdString(targetImagePath.string());
  const QByteArray format =
      targetImagePath.extension() == ".png" ? "PNG" : "JPEG";

  if (!coverImage.save(savePath, format.constData())) {
    return std::unexpected(
        QStringLiteral("Не удалось сохранить изображение по указанному пути."));
  }

  return {};
}
