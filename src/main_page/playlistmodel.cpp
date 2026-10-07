#include "playlistmodel.h"

namespace {
const auto trackTitleRoleName = "trackTitle";
const auto trackArtistRoleName = "trackArtist";
const auto coverImagePathRoleName = "coverImagePath";
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
      {TrackTitle, trackTitleRoleName},
      {TrackArtist, trackArtistRoleName},
      {CoverImagePath, coverImagePathRoleName},
  };
}