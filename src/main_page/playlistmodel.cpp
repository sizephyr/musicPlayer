#include "playlistmodel.h"

namespace {
namespace RoleNames {
const auto uid = "uid";
const auto path = "path";
const auto trackTitle = "trackTitle";
const auto trackArtist = "trackArtist";
const auto coverImagePath = "coverImagePath";
}  // namespace RoleNames
}  // namespace

PlayListModel::PlayListModel(QObject* parent)
    : QAbstractListModel{parent}, _currentTrack(nullptr), _editState(false) {}

void PlayListModel::pushBack(std::shared_ptr<Common::Mp3Description> descr) {
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  _container.push_back(std::move(descr));
  endInsertRows();
}

void PlayListModel::remove(QUuid uid) {
  auto it = std::ranges::find_if(
      _container, [&uid](const auto& descr) { return descr->uid() == uid; });

  if (it != _container.end()) {
    int row = std::distance(_container.begin(), it);

    beginRemoveRows(QModelIndex(), row, row);
    _container.erase(it);
    endRemoveRows();
  }
}

void PlayListModel::reset() { _container.clear(); }

bool PlayListModel::empty() const { return _container.empty(); }

int PlayListModel::rowCount(const QModelIndex& parent) const {
  return _container.size();
}

QVariant PlayListModel::data(const QModelIndex& index, int role) const {
  switch (role) {
    case Uid:
      return QVariant::fromValue(_container[index.row()]->uid());
    case Path:
      return QVariant::fromValue(_container[index.row()]->path().c_str());
    case TrackTitle:
      return QVariant::fromValue(_container[index.row()]->title().value);
    case TrackArtist:
      return QVariant::fromValue(_container[index.row()]->artist().value);
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
  };
}

std::shared_ptr<Common::Mp3Description> PlayListModel::mp3DescriptionByTrackUid(
    QUuid uid) const {
  auto findResult = std::ranges::find_if(
      _container,
      [uid](const auto& description) { return description->uid() == uid; });

  if (findResult == _container.end()) {
    return nullptr;
  }

  return *findResult;
}

std::shared_ptr<Common::Mp3Description> PlayListModel::next(
    QUuid currentDescrUid) {
  auto it =
      std::ranges::find_if(_container, [currentDescrUid](const auto& item) {
        return item->uid() == currentDescrUid;
      });

  if (it == _container.end() || std::next(it) == _container.end()) {
    return nullptr;
  }

  return *std::next(it);
}

bool PlayListModel::nextTrackAvailable(QUuid currentDescrUid) {
  auto it =
      std::ranges::find_if(_container, [currentDescrUid](const auto& item) {
        return item->uid() == currentDescrUid;
      });

  if (it == _container.end() || std::next(it) == _container.end()) {
    return false;
  }

  return true;
}

std::shared_ptr<Common::Mp3Description> PlayListModel::previous(
    QUuid currentDescrUid) {
  auto it =
      std::ranges::find_if(_container, [currentDescrUid](const auto& item) {
        return item->uid() == currentDescrUid;
      });

  if (it == _container.end() || it == _container.begin()) {
    return nullptr;
  }

  return *std::prev(it);
}

bool PlayListModel::previousTrackAvailable(QUuid currentDescrUid) {
  auto it =
      std::ranges::find_if(_container, [currentDescrUid](const auto& item) {
        return item->uid() == currentDescrUid;
      });

  if (it == _container.end() || it == _container.begin()) {
    return false;
  }

  return true;
}

std::shared_ptr<Common::Mp3Description> PlayListModel::currentTrack() const {
  return _currentTrack;
}

void PlayListModel::setCurrentTrack(
    std::shared_ptr<Common::Mp3Description> newCurrentTrack) {
  _currentTrack = newCurrentTrack;
  emit currentTrackChanged();
}

void PlayListModel::resetCurrentTrack() {
  _currentTrack.reset();
  emit currentTrackChanged();
}

QUuid PlayListModel::currentTrackUid() const {
  return _currentTrack ? _currentTrack->uid() : QUuid();
}

bool PlayListModel::editState() const { return _editState; }

void PlayListModel::setEditState(bool newEditState) {
  if (_editState == newEditState) return;
  _editState = newEditState;
  emit editStateChanged();
}
