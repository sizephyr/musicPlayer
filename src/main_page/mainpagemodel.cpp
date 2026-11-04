#include "mainpagemodel.h"

#include <filemanagebackend.h>
#include <utils/utils.h>

#include <QLoggingCategory>
#ifdef Q_OS_MAC
#include "macosplaycontext.h"
#endif

namespace {
const auto loggingCategory = QLoggingCategory("main_page.model");
}

namespace MainPage {

Model::Model(QObject* parent)
    : QObject{parent},
      _playListModel(new PlayListModel(this)),
      _playContextModel(new PlayContextModel(this)) {
#ifdef Q_OS_MAC
  MacOsPlayContext::setupRemoteTransportControls(this);
#endif

  connect(this, &Model::played, this, [this](auto uid) {
    _playContextModel->setPreviousTrackAvailability(
        _playListModel->previousTrackAvailable(uid));
    _playContextModel->setNextTrackAvailability(
        _playListModel->nextTrackAvailable(uid));
  });
  connect(_playContextModel, &PlayContextModel::playNext, this,
          qOverload<>(&Model::playNext));
  connect(this, qOverload<>(&Model::playNext), this,
          [this] { playNext(_playListModel->currentTrackUid()); });
  connect(_playContextModel, &PlayContextModel::playPrevious, this,
          qOverload<>(&Model::playPrevious));
  connect(this, qOverload<>(&Model::playPrevious), this,
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
  if (_playListModel == nullptr) {
    qCWarning(loggingCategory)
        << "Попытка удаления, пока модель плейлиста невалидна";
  }
  if (auto currentTrack = _playListModel->currentTrack();
      currentTrack != nullptr && currentTrack->uid() == uid) {
    if (_playListModel->nextTrackAvailable(uid)) {
      play(_playListModel->next(uid)->uid());
    } else {
      emit stopped();
      _playListModel->resetCurrentTrack();
      _playContextModel->resetCurrentTrack();
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

QMediaPlayer* Model::player() const { return _player; }

void Model::setPlayer(QMediaPlayer* newPlayer) {
  _player = newPlayer;

  connect(this, qOverload<>(&Model::play), _player, &QMediaPlayer::play);
  connect(this, qOverload<>(&Model::pause), _player, &QMediaPlayer::pause);
  connect(_player, &QMediaPlayer::mediaStatusChanged, this,
          [this](QMediaPlayer::MediaStatus status) {
            if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
              playNext(_playListModel->currentTrackUid());
            }
          });
  connect(_player, &QMediaPlayer::metaDataChanged, this,
          &Model::onMetaDataChanged);
  connect(_player, &QMediaPlayer::playbackStateChanged, this,
          &Model::onPlaybackStateChanged);
  connect(_player, &QMediaPlayer::positionChanged, this,
          &Model::onPositionChanged);
}

void Model::onFolderSelected(const QString& newCurrentPlayFolder) {
  if (auto filesListExp =
          FileManageBackend::filesSearch(newCurrentPlayFolder)) {
    qCDebug(loggingCategory) << "+";
    for (const auto& filepath : filesListExp.value()) {
      collectFile(filepath);
    }
  } else {
    qCWarning(loggingCategory)
        << "Не удалось получить список файлов:" << filesListExp.error();
  }
}

void Model::onFileSelected(const QString& sellectedFilePath) {
  if (auto filesListExp = FileManageBackend::collectMp3(sellectedFilePath)) {
    for (const auto& filepath : filesListExp.value()) {
      collectFile(filepath);
    }
  } else {
    qCWarning(loggingCategory)
        << "Не удалось получить корректный путь до файла:"
        << filesListExp.error();
  }
}

void Model::onPositionChanged(qint64 positionMs) {
  const bool isPlaying = _player->playbackState() == QMediaPlayer::PlayingState;
  static qint64 lastSystemPositionMs = 0;
  auto diff = std::abs(positionMs - lastSystemPositionMs);

  if (diff > 1500) {
    double elapsedSeconds = static_cast<double>(positionMs) / 1000.0;
#ifdef Q_OS_MAC
    MacOsPlayContext::updatePlaybackState(elapsedSeconds, isPlaying);
#endif
  }

  if (isPlaying) {
    lastSystemPositionMs = positionMs;
  }
}

void Model::onMetaDataChanged() {
  auto title = _player->metaData().stringValue(QMediaMetaData::Title);
  if (title.isEmpty()) {
    title = _player->source().fileName();
  }

  double durationSeconds = static_cast<double>(_player->duration()) / 1000.0;

#ifdef Q_OS_MAC
  MacOsPlayContext::updateTrackMetadata(title, durationSeconds);
  if (_playContextModel->nextTrackAvailable()) {
    MacOsPlayContext::enablePalyNextOption();
  } else {
    MacOsPlayContext::disablePalyNextOption();
  }
  if (_playContextModel->previousTrackAvailable()) {
    MacOsPlayContext::enablePalyPreviousOption();
  } else {
    MacOsPlayContext::disablePalyPreviousOption();
  }
#endif
}

void Model::onPlaybackStateChanged(QMediaPlayer::PlaybackState newState) {
  auto position = _player->position();
  double elapsedSeconds = static_cast<double>(position) / 1000.0;
  bool isPlaying = (newState == QMediaPlayer::PlayingState);

#ifdef Q_OS_MAC
  MacOsPlayContext::updatePlaybackState(elapsedSeconds, isPlaying);
#endif
}

}  // namespace MainPage
