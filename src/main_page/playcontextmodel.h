#pragma once

#include <mp3description.h>

#include <QCoroTask>
#include <QObject>
#include <QQmlEngine>
#include <QTemporaryFile>

class PlayContextModel : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(QString coverSource READ coverSource NOTIFY coverSourceChanged)
  Q_PROPERTY(bool contextCompletted READ contextCompletted NOTIFY
                 contextComplettedChanged)
 public:
  explicit PlayContextModel(QObject* parent = nullptr);

  QString coverSource() const;
  void setCurrentTrack(std::shared_ptr<Common::Mp3Description> newCurrentTrack);
  void resetCurrentTrack();

  bool contextCompletted() const;
  void setContextCompletted(bool newContextCompletted);

 signals:
  void callCoverSourceProviding();
  void coverSourceChanged();
  void contextComplettedChanged();
  void playNext();
  void playPrevious();

 private slots:
  [[nodiscard]] QCoro::Task<> provideCover();

 private:  // NOLINT
  bool _contextCompletted;
  std::shared_ptr<Common::Mp3Description> _currentTrack{nullptr};
  QTemporaryFile* _currentTrackCover{nullptr};
};
