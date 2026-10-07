#pragma once

#include <stringentityid.h>

#include <QString>
#include <QUrl>
#include <QUuid>

namespace Common {

namespace Mp3DescriptionTypes {
using TrackTitle = StringEntityId<struct TrackTitleTag>;
using TrackArtist = StringEntityId<struct TrackArtistTag>;
}  // namespace Mp3DescriptionTypes

class Mp3Description {
 public:
  Mp3Description(Mp3DescriptionTypes::TrackTitle,
                 Mp3DescriptionTypes::TrackArtist,
                 std::optional<QUrl> = std::nullopt);

  QUuid uid() const;
  Mp3DescriptionTypes::TrackTitle title() const;
  Mp3DescriptionTypes::TrackArtist artist() const;
  std::optional<QUrl> coverImagePath() const;

 private:
  QUuid _uid;
  Mp3DescriptionTypes::TrackTitle _title;
  Mp3DescriptionTypes::TrackArtist _artist;
  std::optional<QUrl> _coverImagePath;
};

}  // namespace Common
