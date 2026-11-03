#pragma once

#include <QMediaPlayer>

class MacOsPlayContext {
 public:
  static void setupRemoteTransportControls(QMediaPlayer* player);

  // Вызывать только при смене трека (низкая частота вызовов)
  static void updateTrackMetadata(const QString& title, double durationSeconds);

  // Вызывать при явной остановке, запуске или ручной перемотке трека
  // пользователем
  static void updatePlaybackState(double elapsedSeconds, bool isPlaying);
};
