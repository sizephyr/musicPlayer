#include "macosplaycontext.h"
#include "mainpagemodel.h"

#import <MediaPlayer/MediaPlayer.h>

void MacOsPlayContext::setupRemoteTransportControls(MainPage::Model* mainPageModel) {
  // Получение доступа к системному центру команд
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];

  [commandCenter.playCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        emit mainPageModel->play();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  [commandCenter.pauseCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        emit mainPageModel->pause();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  [commandCenter.nextTrackCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        emit mainPageModel->playNext();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  [commandCenter.previousTrackCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        emit mainPageModel->playPrevious();
        return MPRemoteCommandHandlerStatusSuccess;
      }];

  [commandCenter.changePlaybackPositionCommand
      addTargetWithHandler:^MPRemoteCommandHandlerStatus(MPRemoteCommandEvent* _Nonnull event) {
        MPChangePlaybackPositionCommandEvent* positionEvent =
            (MPChangePlaybackPositionCommandEvent*)event;
        // Использование корректного свойства positionTime
        qint64 positionMs = static_cast<qint64>(positionEvent.positionTime * 1000.0);
        mainPageModel->player()->setPosition(positionMs);
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

void MacOsPlayContext::enablePalyNextOption() {
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
  commandCenter.nextTrackCommand.enabled = YES;
}

void MacOsPlayContext::disablePalyNextOption() {
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
  commandCenter.nextTrackCommand.enabled = NO;
}

void MacOsPlayContext::enablePalyPreviousOption() {
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
  commandCenter.previousTrackCommand.enabled = YES;
}

void MacOsPlayContext::disablePalyPreviousOption() {
  MPRemoteCommandCenter* commandCenter = [MPRemoteCommandCenter sharedCommandCenter];
  commandCenter.previousTrackCommand.enabled = NO;
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
