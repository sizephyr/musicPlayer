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
  Q_PROPERTY(bool contextCompletted READ contextCompletted WRITE
                 setContextCompletted NOTIFY contextComplettedChanged)
  Q_PROPERTY(bool previousTrackAvailable READ previousTrackAvailable NOTIFY
                 previousTrackAvailableChanged)
  Q_PROPERTY(bool nextTrackAvailable READ nextTrackAvailable NOTIFY
                 nextTrackAvailableChanged)
 public:
  explicit PlayContextModel(QObject* parent = nullptr);

  [[nodiscard]] QString coverSource() const;
  void setCurrentTrack(std::shared_ptr<Common::Mp3Description> newCurrentTrack);
  void resetCurrentTrack();

  [[nodiscard]] bool contextCompletted() const;
  void setContextCompletted(bool newContextCompletted);

  [[nodiscard]] bool previousTrackAvailable() const;
  void setPreviousTrackAvailability(bool);
  [[nodiscard]] bool nextTrackAvailable() const;
  void setNextTrackAvailability(bool);

 signals:
  void callCoverSourceProviding();
  void coverSourceChanged();
  void contextComplettedChanged();
  void playNext();
  void playPrevious();
  void previousTrackAvailableChanged();
  void nextTrackAvailableChanged();

 private slots:
  [[nodiscard]] QCoro::Task<> provideCover();

 private:  // NOLINT
  bool _contextCompletted;
  std::shared_ptr<Common::Mp3Description> _currentTrack{nullptr};
  QTemporaryFile* _currentTrackCover{nullptr};
  bool _previousTrackAvailable;
  bool _nextTrackAvailable;
};
