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
  connect(this, &Model::played, this, [this](auto uid) {
    _playContextModel->setPreviousTrackAvailability(
        _playListModel->previousTrackAvailable(uid));
    _playContextModel->setNextTrackAvailability(
        _playListModel->nextTrackAvailable(uid));
  });
  connect(_playContextModel, &PlayContextModel::playNext, this,
          [this] { playNext(_playListModel->currentTrackUid()); });
  connect(_playContextModel, &PlayContextModel::playPrevious, this,
          [this] { playPrevious(_playListModel->currentTrackUid()); });
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

void Model::remove(QUuid uid) {
  if (_playListModel->currentTrack()->uid() == uid) {
    if (_playListModel->nextTrackAvailable(uid)) {
      play(_playListModel->next(uid)->uid());
    } else {
      emit stopped();
    }
  }
  _playListModel->remove(uid);
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

void Model::collectFile(const std::filesystem::path& filepath) {
  if (auto fileDescription = FileManageBackend::parseMp3(filepath)) {
    _playListModel->pushBack(fileDescription);
  } else {
    qCWarning(loggingCategory)
        << "Не удалось распарсить описание файла" << filepath.c_str();
  }
}

void Model::setPlayer(QMediaPlayer* newPlayer) {
  _player = newPlayer;
  connect(_player, &QMediaPlayer::mediaStatusChanged, this,
          [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
              playNext(_playListModel->currentTrackUid());
            }
          });
}

void Model::onFolderSelected(const QString& newCurrentPlayFolder) {
  if (auto filesListExp =
          FileManageBackend::filesSearch(newCurrentPlayFolder)) {
    for (const auto& filepath : filesListExp.value()) {
      collectFile(filepath);
    }
  } else {
    qCWarning(loggingCategory)
        << "Не удалось получить список файлов:" << filesListExp.error();
  }
}

void Model::onFileSelected(const QString& sellectedFilePath) {
  // по сути, это нецелевое использование метода FileManageBackend::filesSearch,
  // но пока что пусть
  if (auto filesListExp = FileManageBackend::filesSearch(sellectedFilePath)) {
    for (const auto& filepath : filesListExp.value()) {
      collectFile(filepath);
    }
  } else {
    qCWarning(loggingCategory)
        << "Не удалось получить корректный путь до файла:"
        << filesListExp.error();
  }
}

}  // namespace MainPage
