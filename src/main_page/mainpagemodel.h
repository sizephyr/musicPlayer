#pragma once

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

  Q_PROPERTY(PlayListModel* playListModel READ playListModel NOTIFY
                 playListModelChanged)
  Q_PROPERTY(QString currentPlayFolder READ currentPlayFolder WRITE
                 setCurrentPlayFolder NOTIFY currentPlayFolderChanged)
 public:
  explicit Model(QObject* parent = nullptr);

  [[nodiscard]] QString currentPlayFolder() const;
  void setCurrentPlayFolder(const QString& newCurrentPlayFolder);

  PlayListModel* playListModel() const;
  [[nodiscard]] Q_INVOKABLE QString trackPathOf(QUuid);

 public slots:
  [[nodiscard]] QCoro::Task<> provideCoverFor(QUuid);

 signals:
  void currentPlayFolderChanged();
  void playListModelChanged();
  void coverProvided(const QString&);

 private slots:
  void onCurrentPlayFolderChanged();

 private:  // NOLINT
  QString _currentPlayFolder;
  PlayListModel* _playListModel{nullptr};
};

}  // namespace MainPage