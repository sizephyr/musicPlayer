#pragma once

#include <mp3description.h>

#include <QCoroTask>
#include <QString>
#include <expected>
#include <filesystem>

class FileManageBackend {
 public:
  [[nodiscard]] static std::expected<QList<std::filesystem::path>, QString>
  filesSearch(const QString& path);
  [[nodiscard]] static std::shared_ptr<Common::Mp3Description> parseMp3(
      const std::filesystem::path& filePath);
  /// \brief попытка изъять обложку с помощью ffmpeg, потому что она прячется
  /// как долбанный видеопоток!
  [[nodiscard]] static QCoro::Task<> fillCoverImagePathByFfmpegCLI(
      std::shared_ptr<const Common::Mp3Description>, std::filesystem::path);

 private:
  [[nodiscard]] static std::shared_ptr<Common::Mp3Description> parseID3(
      const std::filesystem::path& filePath);
  [[nodiscard]] static std::optional<std::filesystem::path> getCoverImagePath(
      QByteArrayView frameData);
};
