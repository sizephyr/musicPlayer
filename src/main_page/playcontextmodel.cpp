#include "playcontextmodel.h"

#include <filemanagebackend.h>

#include <QDir>

PlayContextModel::PlayContextModel(QObject* parent)
    : QObject{parent},
      _currentTrackCover(new QTemporaryFile(
          QDir::tempPath() + "/current_cover_XXXXXX.png", this)) {
  connect(this, &PlayContextModel::callCoverSourceProviding, this,
          &PlayContextModel::provideCover);
}

QString PlayContextModel::coverSource() const {
  return QStringLiteral("file://") + _currentTrackCover->fileName();
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
  if (_contextCompletted == newContextCompletted) return;
  _contextCompletted = newContextCompletted;
  emit contextComplettedChanged();
}

QCoro::Task<> PlayContextModel::provideCover() {
  if (_currentTrack != nullptr && _currentTrackCover->open()) {
    qDebug() << "::::" << _currentTrackCover->fileName()
             << _currentTrackCover->filesystemFileName().c_str();
    auto cleanup = qScopeGuard([this] { _currentTrackCover->close(); });
    // попытка изъять обложку с помощью ffmpeg, потому что она прячется
    //  как долбанный видеопоток!
    co_await FileManageBackend::fillCoverImagePathByFfmpegCLI(
        _currentTrack, _currentTrackCover->filesystemFileName());
  }

  emit coverSourceChanged();
}
