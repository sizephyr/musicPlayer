#include "playcontextmodel.h"

#include <filemanagebackend.h>

#include <QDir>
#include <QLoggingCategory>

namespace {
const auto loggingCategory = QLoggingCategory("play_context_model");
}

PlayContextModel::PlayContextModel(QObject* parent)
    : QObject{parent},
      _currentTrackCover(new QTemporaryFile(
          QDir::tempPath() + "/current_cover_XXXXXX.png", this)) {
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
    qDebug() << "::::" << _currentTrackCover->fileName()
             << _currentTrackCover->filesystemFileName().c_str();
    auto cleanup = qScopeGuard([this] { _currentTrackCover->close(); });
    // попытка изъять обложку с помощью ffmpeg, потому что она прячется
    // как долбаный видеопоток!
    if (auto fillingResult =
            co_await FileManageBackend::fillCoverImagePathByFfmpegCLI(
                _currentTrack, _currentTrackCover->filesystemFileName());
        !fillingResult) {
      qCWarning(loggingCategory) << fillingResult.error();
      _currentTrackCover->remove();
    }
  }

  emit coverSourceChanged();
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
