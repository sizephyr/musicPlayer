#pragma once

#include <playlistmodel.h>

#include <QObject>
#include <QQmlEngine>

namespace MainPage {

class Model : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(MainPageModel)

  Q_PROPERTY(PlayListModel* playListModel READ playListModel NOTIFY
                 playListModelChanged)
  Q_PROPERTY(QString currentPlayFolder READ currentPlayFolder WRITE
                 setCurrentPlayFolder NOTIFY currentPlayFolderChanged)
 public:
  explicit Model(QObject* parent = nullptr);

  [[nodiscard]] QString currentPlayFolder() const;
  void setCurrentPlayFolder(const QString& newCurrentPlayFolder);

  PlayListModel* playListModel() const;

 signals:
  void currentPlayFolderChanged();

  void playListModelChanged();

 private slots:
  void onCurrentPlayFolderChanged();

 private:  // NOLINT
  QString _currentPlayFolder;
  PlayListModel* _playListModel{nullptr};
};

}  // namespace MainPage
