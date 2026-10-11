#pragma once

#include <playcontextmodel.h>
#include <playlistmodel.h>

#include <QCoroTask>
#include <QImage>
#include <QMediaMetaData>
#include <QObject>
#include <QQmlEngine>

namespace MainPage {
class CoverProvider;

class Model : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(MainPageModel)

  Q_PROPERTY(PlayListModel* playListModel READ playListModel CONSTANT)
  Q_PROPERTY(PlayContextModel* playContextModel READ playContextModel CONSTANT)
  Q_PROPERTY(QString currentPlayFolder READ currentPlayFolder WRITE
                 setCurrentPlayFolder NOTIFY currentPlayFolderChanged)
 public:
  explicit Model(QObject* parent = nullptr);

  [[nodiscard]] QString currentPlayFolder() const;
  void setCurrentPlayFolder(const QString& newCurrentPlayFolder);

  [[nodiscard]] PlayListModel* playListModel() const;
  [[nodiscard]] PlayContextModel* playContextModel() const;
  [[nodiscard]] Q_INVOKABLE QString trackPathOf(QUuid);
  Q_INVOKABLE void play(QUuid uid);

 signals:
  void currentPlayFolderChanged();
  void coverProvided(const QString&);
  void played(QUuid);

 private slots:
  void onCurrentPlayFolderChanged();

 private:  // NOLINT
  void playNext(QUuid uid);
  void playPrevious(QUuid uid);

  QString _currentPlayFolder;
  PlayListModel* _playListModel{nullptr};
  PlayContextModel* _playContextModel{nullptr};
};

}  // namespace MainPage