#pragma once

#include <playcontextmodel.h>
#include <playlistmodel.h>

#include <QCoroTask>
#include <QImage>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QObject>
#include <QQmlEngine>

namespace MainPage {
class CoverProvider;

class Model : public QObject {
  Q_OBJECT
  QML_NAMED_ELEMENT(MainPageModel)

  Q_PROPERTY(PlayListModel* playListModel READ playListModel CONSTANT)
  Q_PROPERTY(PlayContextModel* playContextModel READ playContextModel CONSTANT)

 public:
  explicit Model(QObject* parent = nullptr);

  [[nodiscard]] PlayListModel* playListModel() const;
  [[nodiscard]] PlayContextModel* playContextModel() const;
  [[nodiscard]] Q_INVOKABLE QString trackPathOf(QUuid);
  Q_INVOKABLE void play(QUuid uid);
  Q_INVOKABLE void remove(QUuid uid);

  /// \brief для установки юайного синглтона в контекст
  Q_INVOKABLE void setPlayer(QMediaPlayer* newPlayer);

 signals:
  void coverProvided(const QString&);
  void played(QUuid);
  void stopped();

 public slots:
  void onFolderSelected(const QString& newCurrentPlayFolder);
  void onFileSelected(const QString& sellectedFilePath);

 private:  // NOLINT
  void playNext(QUuid uid);
  void playPrevious(QUuid uid);
  void collectFile(const std::filesystem::path&);

  PlayListModel* _playListModel{nullptr};
  PlayContextModel* _playContextModel{nullptr};
  QMediaPlayer* _player{nullptr};
};

}  // namespace MainPage