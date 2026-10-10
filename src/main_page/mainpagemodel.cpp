#include "mainpagemodel.h"

#include <filemanagebackend.h>
#include <utils/utils.h>

#include <QLoggingCategory>

namespace {
const auto loggingCategory = QLoggingCategory("main_page.model");
}

namespace MainPage {

Model::Model(QObject* parent)
    : QObject{parent}, _playListModel(new PlayListModel(this)) {}

QString Model::currentPlayFolder() const { return _currentPlayFolder; }

void Model::setCurrentPlayFolder(const QString& newCurrentPlayFolder) {
  if (_currentPlayFolder == newCurrentPlayFolder) {
    return;
  }
  _currentPlayFolder = newCurrentPlayFolder;
  onCurrentPlayFolderChanged();
  if (!_playListModel->empty()) {
    emit currentPlayFolderChanged();
  }
}

void Model::onCurrentPlayFolderChanged() {
  if (auto filesListExp = FileManageBackend::filesSearch(_currentPlayFolder)) {
    for (const auto& filepath : filesListExp.value()) {
      if (auto fileDescriptionOpt = FileManageBackend::parseMp3(filepath)) {
        auto fileDescription = fileDescriptionOpt.value();
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

QString Model::trackPathOf(QUuid trackUid) {
  if (auto descrOpt = _playListModel->mp3DescriptionByTrackUid(trackUid)) {
    return descrOpt->path().c_str();
  }
  return "";
}

QCoro::Task<> Model::provideCoverFor(QUuid trackUid) {
  if (auto descr = _playListModel->mp3DescriptionByTrackUid(trackUid)) {
    auto fileDescription = descr.value();
    if (!fileDescription.coverImagePath().has_value()) {
      // попытка изъять обложку с помощью ffmpeg, потому что она прячется
      //  как долбанный видеопоток!
      co_await FileManageBackend::fillCoverImagePathByFfmpegCLI(
          fileDescription);
    }
    emit coverProvided(fileDescription.coverImagePath().value_or("").c_str());
  } else {
    emit coverProvided("");
  }
}

}  // namespace MainPage
