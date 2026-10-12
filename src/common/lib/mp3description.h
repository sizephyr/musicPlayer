#pragma once

#include <stringentityid.h>

#include <QString>
#include <QUrl>
#include <QUuid>
#include <filesystem>

namespace Common {

namespace Mp3DescriptionTypes {
using TrackTitle = StringEntityId<struct TrackTitleTag>;
using TrackArtist = StringEntityId<struct TrackArtistTag>;
}  // namespace Mp3DescriptionTypes

class Mp3Description {
 public:
     Mp3Description(std::filesystem::path,
                    Mp3DescriptionTypes::TrackTitle,
                    Mp3DescriptionTypes::TrackArtist);

     [[nodiscard]] QUuid uid() const;
     [[nodiscard]] std::filesystem::path path() const;
     [[nodiscard]] Mp3DescriptionTypes::TrackTitle title() const;
     [[nodiscard]] Mp3DescriptionTypes::TrackArtist artist() const;

     [[nodiscard]] auto operator<=>(const Mp3Description &) const = default;

 private:
  QUuid _uid;
  std::filesystem::path _path;
  Mp3DescriptionTypes::TrackTitle _title;
  Mp3DescriptionTypes::TrackArtist _artist;
};

}  // namespace Common
