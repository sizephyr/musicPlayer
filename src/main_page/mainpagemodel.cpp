#include "mainpagemodel.h"

#include <filemanagebackend.h>

#include <QLoggingCategory>

namespace {
const auto loggingCategory = QLoggingCategory("main_page.model");
}

namespace MainPage {

Model::Model(QObject* parent)
    : QObject{parent}, _playListModel(new PlayListModel(this)) {
  connect(this, &Model::currentPlayFolderChanged, this,
          &Model::onCurrentPlayFolderChanged);
}

QString Model::currentPlayFolder() const { return _currentPlayFolder; }

void Model::setCurrentPlayFolder(const QString& newCurrentPlayFolder) {
  if (_currentPlayFolder == newCurrentPlayFolder) return;
  _currentPlayFolder = newCurrentPlayFolder;
  emit currentPlayFolderChanged();
}

void Model::onCurrentPlayFolderChanged() {
  if (auto filesListExp = FileManageBackend::filesSearch(_currentPlayFolder)) {
    for (const auto& filepath : filesListExp.value()) {
      if (auto fileDescriptionOpt = FileManageBackend::parseMp3(filepath)) {
        _playListModel->pushBack(fileDescriptionOpt.value());
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

}  // namespace MainPage
