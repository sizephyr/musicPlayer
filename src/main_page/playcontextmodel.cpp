#include "playcontextmodel.h"

#include <filemanagebackend.h>

PlayContextModel::PlayContextModel(QObject* parent) : QObject{parent} {
  connect(this, &PlayContextModel::callCoverSurceProviding, this,
          &PlayContextModel::provideCover);
}

QString PlayContextModel::coverSource() const {
  if (_currentTrack == nullptr) {
    return "";
  }

  auto pathString =
      QString(_currentTrack->coverImagePath().value_or({}).c_str());
  if (pathString.isEmpty()) {
    return "";
  }

  return QStringLiteral("file://") + pathString;
}

void PlayContextModel::setCurrentTrack(
    std::shared_ptr<Common::Mp3Description> newCurrentTrack) {
  if (_currentTrack == newCurrentTrack) {
    return;
  }
  _currentTrack = newCurrentTrack;
  emit callCoverSurceProviding();
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
  qDebug() << "::::" << __FUNCTION__;
  if (_currentTrack != nullptr) {
    if (!_currentTrack->coverImagePath().has_value()) {
      // попытка изъять обложку с помощью ffmpeg, потому что она прячется
      //  как долбанный видеопоток!
      co_await FileManageBackend::fillCoverImagePathByFfmpegCLI(_currentTrack);
    }
  }

  emit coverSourceChanged();
}
