#include "playcontextmodel.h"

#include <filemanagebackend.h>

#include <QDir>
#include <QLoggingCategory>
#include <QMediaMetaData>

#ifdef Q_OS_MAC
#include "macosplaycontext.h"
#endif

namespace {
const auto loggingCategory = QLoggingCategory("play_context_model");
}

PlayContextModel::PlayContextModel(QObject* parent)
    : QObject{parent},
      _contextCompletted(false),
      _currentTrack(nullptr),
      _currentTrackCover(new QTemporaryFile(
          QDir::tempPath() + "/current_cover_XXXXXX.png", this)),
      _previousTrackAvailable(false),
      _nextTrackAvailable(false),
      _playerContext(nullptr) {
  connect(this, &PlayContextModel::callCoverSourceProviding, this,
          &PlayContextModel::provideCover);
}

QString PlayContextModel::coverSource() const {
  if (_currentTrackCover->exists()) {
    return QStringLiteral("file://") + _currentTrackCover->fileName();
  }
  return "";
}

void PlayContextModel::setCurrentTrack(
    std::shared_ptr<Common::Mp3Description> newCurrentTrack) {
  if (_currentTrack == newCurrentTrack) {
    return;
  }
  _currentTrack = newCurrentTrack;
  emit callCoverSourceProviding();
}

void PlayContextModel::resetCurrentTrack() {
  _currentTrack.reset();
  _currentTrackCover->remove();
  emit coverSourceChanged();
}

bool PlayContextModel::contextCompletted() const { return _contextCompletted; }

void PlayContextModel::setContextCompletted(bool newContextCompletted) {
  if (_contextCompletted == newContextCompletted) {
    return;
  }
  _contextCompletted = newContextCompletted;
  emit contextComplettedChanged();
}

QCoro::Task<> PlayContextModel::provideCover() {
  if (_currentTrack != nullptr && _currentTrackCover->open()) {
    auto cleanup = qScopeGuard([this] { _currentTrackCover->close(); });
    if (auto tagLibFillingResult =
            FileManageBackend::fillCoverImagePathByTagLib(
                _currentTrack, _currentTrackCover->filesystemFileName())) {
      emit coverSourceChanged();
    }
    // попытка изъять обложку с помощью ffmpeg, потому что она может
    // прячетаться как долбаный видеопоток!
    else if (auto ffmpegFillingResult =
                 co_await FileManageBackend::fillCoverImagePathByFfmpegCLI(
                     _currentTrack, _currentTrackCover->filesystemFileName())) {
      emit coverSourceChanged();
    } else {
      qCWarning(loggingCategory)
          << tagLibFillingResult.error() << ffmpegFillingResult.error();
      _currentTrackCover->remove();
      emit coverSourceChanged();
    }
  }
}

bool PlayContextModel::previousTrackAvailable() const {
  return _previousTrackAvailable;
}

void PlayContextModel::setPreviousTrackAvailability(bool isAvailable) {
  _previousTrackAvailable = isAvailable;
  emit previousTrackAvailableChanged();
}

bool PlayContextModel::nextTrackAvailable() const {
  return _nextTrackAvailable;
}

void PlayContextModel::setNextTrackAvailability(bool isAvailable) {
  _nextTrackAvailable = isAvailable;
  emit nextTrackAvailableChanged();
}

#ifdef Q_OS_MAC
void PlayContextModel::setPlayerContext(QMediaPlayer* playerContext) {
  if (playerContext == nullptr) {
    qCWarning(loggingCategory)
        << "Попытка установить контекст невалидного медиаплеера";
    return;
  }
  _playerContext = playerContext;
  MacOsPlayContext::setupRemoteTransportControls(playerContext);

  connect(_playerContext, &QMediaPlayer::metaDataChanged, this, [this]() {
    auto title = _playerContext->metaData().stringValue(QMediaMetaData::Title);
    if (title.isEmpty()) {
      title = _playerContext->source().fileName();
    }

    double durationSeconds =
        static_cast<double>(_playerContext->duration()) / 1000.0;

    MacOsPlayContext::updateTrackMetadata(title, durationSeconds);
  });

  connect(_playerContext, &QMediaPlayer::playbackStateChanged, this,
          [position = _playerContext->position()](auto state) {
            double elapsedSeconds = static_cast<double>(position) / 1000.0;
            bool isPlaying = (state == QMediaPlayer::PlayingState);

            MacOsPlayContext::updatePlaybackState(elapsedSeconds, isPlaying);
          });

  connect(_playerContext, &QMediaPlayer::positionChanged, this,
          [isPlaying = _playerContext->playbackState() ==
                       QMediaPlayer::PlayingState](auto positionMs) {
            static qint64 lastSystemPositionMs = 0;
            auto diff = std::abs(positionMs - lastSystemPositionMs);

            if (diff > 1500) {
              double elapsedSeconds = static_cast<double>(positionMs) / 1000.0;
              MacOsPlayContext::updatePlaybackState(elapsedSeconds, isPlaying);
            }

            if (isPlaying) {
              lastSystemPositionMs = positionMs;
            }
          });
}
#elif Q_OS_LINUX
/// \todo
#endif
