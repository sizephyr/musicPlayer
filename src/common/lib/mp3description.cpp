#include "mp3description.h"

namespace Common {

Mp3Description::Mp3Description(std::filesystem::path path,
                               Mp3DescriptionTypes::TrackTitle title,
                               Mp3DescriptionTypes::TrackArtist artist)
    : _uid(QUuid::createUuid()),
      _path(std::move(path)),
      _title(std::move(title)),
      _artist(std::move(artist)) {}

QUuid Mp3Description::uid() const { return _uid; }

std::filesystem::path Mp3Description::path() const { return _path; }

Mp3DescriptionTypes::TrackTitle Mp3Description::title() const { return _title; }

Mp3DescriptionTypes::TrackArtist Mp3Description::artist() const {
  return _artist;
}

}  // namespace Common
