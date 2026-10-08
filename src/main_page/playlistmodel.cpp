#include "playlistmodel.h"

#include <ranges>

namespace {
namespace RoleNames {
const auto uid = "uid";
const auto path = "path";
const auto trackTitle = "trackTitle";
const auto trackArtist = "trackArtist";
const auto coverImagePath = "coverImagePath";
}  // namespace RoleNames
}  // namespace

PlayListModel::PlayListModel(QObject* parent) : QAbstractListModel{parent} {}

void PlayListModel::pushBack(Common::Mp3Description descr) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  _container.push_back(std::move(descr));
  endInsertRows();
}

void PlayListModel::reset() { _container.clear(); }

int PlayListModel::rowCount(const QModelIndex& parent) const {
  return _container.size();
}

QVariant PlayListModel::data(const QModelIndex& index, int role) const {
  switch (role) {
    case Uid:
      return QVariant::fromValue(_container[index.row()].uid());
    case Path:
      return QVariant::fromValue(_container[index.row()].path().c_str());
    case TrackTitle:
      return QVariant::fromValue(_container[index.row()].title().value);
    case TrackArtist:
      return QVariant::fromValue(_container[index.row()].artist().value);
    case CoverImagePath:
      return QVariant::fromValue(
          _container[index.row()].coverImagePath().value_or(QUrl()).toString());
    default:
      return QVariant();
  }
}

QHash<int, QByteArray> PlayListModel::roleNames() const {
  return {
      {Uid, RoleNames::uid},
      {Path, RoleNames::path},
      {TrackTitle, RoleNames::trackTitle},
      {TrackArtist, RoleNames::trackArtist},
      {CoverImagePath, RoleNames::coverImagePath},
  };
}

std::optional<Common::Mp3Description> PlayListModel::mp3DescriptionByTrackUid(
    QUuid uid) const {
  auto findResult = std::ranges::find_if(
      _container,
      [uid](const auto& description) { return description.uid() == uid; });

  if (findResult == _container.end()) {
    return std::nullopt;
  }
  return *findResult;
}