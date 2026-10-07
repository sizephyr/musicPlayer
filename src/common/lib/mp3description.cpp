#include "mp3description.h"

namespace Common {

Mp3Description::Mp3Description(std::filesystem::path path,
                               Mp3DescriptionTypes::TrackTitle title,
                               Mp3DescriptionTypes::TrackArtist artist,
                               std::optional<QUrl> coverImagePath)
    : _uid(QUuid::createUuid()),
      _path(std::move(path)),
      _title(std::move(title)),
      _artist(std::move(artist)),
      _coverImagePath(std::move(coverImagePath)) {}

QUuid Mp3Description::uid() const { return _uid; }

Mp3DescriptionTypes::TrackTitle Mp3Description::title() const { return _title; }

Mp3DescriptionTypes::TrackArtist Mp3Description::artist() const {
  return _artist;
}

std::optional<QUrl> Mp3Description::coverImagePath() const {
  return _coverImagePath;
}

}  // namespace Common
