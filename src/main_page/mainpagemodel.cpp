#include "mainpagemodel.h"

#include <filemanagebackend.h>
#include <utils/utils.h>

#include <QLoggingCategory>

namespace {
const auto loggingCategory = QLoggingCategory("main_page.model");
}

namespace MainPage {

Model::Model(QObject* parent)
    : QObject{parent},
      _playListModel(new PlayListModel(this)),
      _playContextModel(new PlayContextModel(this)) {
  // connect(this, &Model::played, this, [this](auto uid) {
  //   _playContextModel->setCurrentTrack(
  //       _playListModel->mp3DescriptionByTrackUid(uid));
  // });
  connect(_playContextModel, &PlayContextModel::playNext, this,
          [this] { playNext(_playListModel->currentTrackUid()); });
  connect(_playContextModel, &PlayContextModel::playPrevious, this,
          [this] { playPrevious(_playListModel->currentTrackUid()); });
}

QString Model::currentPlayFolder() const { return _currentPlayFolder; }

void Model::setCurrentPlayFolder(const QString& newCurrentPlayFolder) {
  if (_currentPlayFolder == newCurrentPlayFolder) {
    return;
  }
  _currentPlayFolder = newCurrentPlayFolder;
  onCurrentPlayFolderChanged();
  if (!_playListModel->empty()) {
    emit currentPlayFolderChanged();
    _playContextModel->setContextCompletted(true);
  } else {
    _playContextModel->setContextCompletted(false);
  }
}

void Model::onCurrentPlayFolderChanged() {
  if (auto filesListExp = FileManageBackend::filesSearch(_currentPlayFolder)) {
    for (const auto& filepath : filesListExp.value()) {
      if (auto fileDescription = FileManageBackend::parseMp3(filepath)) {
        _playListModel->pushBack(fileDescription);
      } else {
        qCWarning(loggingCategory)
            << "Не удалось распарсить описание файла" << filepath.c_str();
      }
    }
  } else {
    qCWarning(loggingCategory)
        << "Не удалось получить список файлов:" << filesListExp.error();
  }
}

PlayListModel* Model::playListModel() const { return _playListModel; }

PlayContextModel* Model::playContextModel() const { return _playContextModel; }

QString Model::trackPathOf(QUuid trackUid) {
  if (auto descrOpt = _playListModel->mp3DescriptionByTrackUid(trackUid)) {
    return descrOpt->path().c_str();
  }
  return "";
}

void Model::play(QUuid uid) {
  if (auto track = _playListModel->mp3DescriptionByTrackUid(uid)) {
    _playListModel->setCurrentTrack(track);
    _playContextModel->setCurrentTrack(track);
    emit played(track->uid());
  } else {
    _playListModel->resetCurrentTrack();
    _playContextModel->resetCurrentTrack();
  }
}

void Model::playNext(QUuid uid) {
  if (auto nextTrack = _playListModel->next(uid)) {
    play(nextTrack->uid());  // тут хорошо бы отправлять сразу весь дескр, но
                             // мне в падлу
  }
}

void Model::playPrevious(QUuid uid) {
  if (auto previousTrack = _playListModel->previous(uid)) {
    play(previousTrack->uid());  // тут хорошо бы отправлять сразу весь дескр,
                                 // но мне в падлу
  }
}

}  // namespace MainPage
