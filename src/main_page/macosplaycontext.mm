#include "macosplaycontext.h"

#import <MediaPlayer/MediaPlayer.h>

void MacOsPlayContext::setupRemoteTransportControls(QMediaPlayer* player) {
  // Получение доступа к системному центру команд
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];

  // Обработка сигнала воспроизведения
  [commandCenter.playCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        player->play();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  // Обработка сигнала паузы
  [commandCenter.pauseCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        player->pause();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  // Обработка изменения положения воспроизведения (перемотки)
  [commandCenter.changePlaybackPositionCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        MPChangePlaybackPositionCommandEvent* positionEvent =
            (MPChangePlaybackPositionCommandEvent*)event;
        // Использование корректного свойства positionTime
        qint64 positionMs = static_cast<qint64>(positionEvent.positionTime * 1000.0);
        player->setPosition(positionMs);
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  commandCenter.playCommand.enabled = YES;
  commandCenter.pauseCommand.enabled = YES;
  commandCenter.changePlaybackPositionCommand.enabled = YES;
}

void MacOsPlayContext::updateTrackMetadata(const QString& title, double durationSeconds) {
  MPNowPlayingInfoCenter* center = [MPNowPlayingInfoCenter defaultCenter];
  NSMutableDictionary* info = center.nowPlayingInfo ? [center.nowPlayingInfo mutableCopy]
                                                    : [NSMutableDictionary dictionary];

  info[MPMediaItemPropertyTitle] = title.toNSString();
  info[MPMediaItemPropertyPlaybackDuration] = @(durationSeconds);

  center.nowPlayingInfo = info;
}

void MacOsPlayContext::updatePlaybackState(double elapsedSeconds, bool isPlaying) {
  MPNowPlayingInfoCenter* center = [MPNowPlayingInfoCenter defaultCenter];
  NSMutableDictionary* info = center.nowPlayingInfo ? [center.nowPlayingInfo mutableCopy]
                                                    : [NSMutableDictionary dictionary];

  info[MPNowPlayingInfoPropertyElapsedPlaybackTime] = @(elapsedSeconds);
  // Передаем скорость: 1.0 заставит систему самой двигать ползунок вперед, 0.0 остановит его
  info[MPNowPlayingInfoPropertyPlaybackRate] = isPlaying ? @(1.0) : @(0.0);

  center.nowPlayingInfo = info;
}
