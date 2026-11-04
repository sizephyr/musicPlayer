#pragma once

#include <QMediaPlayer>

namespace MainPage {
class Model;
}
class MacOsPlayContext {
 public:
  static void setupRemoteTransportControls(MainPage::Model*);

  // Вызывать только при смене трека (низкая частота вызовов)
  static void updateTrackMetadata(const QString& title, double durationSeconds);

  // Вызывать при явной остановке, запуске или ручной перемотке трека
  // пользователем
  static void updatePlaybackState(double elapsedSeconds, bool isPlaying);

  static void enablePalyNextOption();
  static void disablePalyNextOption();
  static void enablePalyPreviousOption();
  static void disablePalyPreviousOption();
};
