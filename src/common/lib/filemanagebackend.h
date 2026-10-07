#pragma once

#include <mp3description.h>

#include <QString>
#include <expected>
#include <filesystem>

class FileManageBackend {
 public:
  [[nodiscard]] static std::expected<QList<std::filesystem::path>, QString>
  filesSearch(const QString& path);
  [[nodiscard]] static std::optional<Common::Mp3Description> parseMp3(
      const std::filesystem::path& filePath);

 private:
  [[nodiscard]] static std::optional<Common::Mp3Description> parseID3(
      const std::filesystem::path& filePath);
  [[nodiscard]] static std::optional<QUrl> getCoverImagePath(
      QByteArrayView frameData);
};
