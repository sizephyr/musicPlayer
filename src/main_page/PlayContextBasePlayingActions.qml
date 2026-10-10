import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import modules.configuration
import modules.main_page

Item {
    id: root

    property int radiusCorrecting

    signal play
    signal pause
    signal playNext
    signal playPrevious

    Rectangle {
        anchors.fill: parent

        radius: Math.max(0, Configuration.r3Radius - root.radiusCorrecting)
        color: Configuration.theme.semiTransparentLightGrayColor

        RowLayout {
            anchors.fill: parent
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                BasePlayingActionsButton {
                    anchors.centerIn: parent
                    width: playPauseButton.width * 0.7
                    height: width
                    icon.source: "img/previous_track_pic.png"
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                BasePlayingActionsButton {
                    id: playPauseButton
                    anchors.centerIn: parent
                    width: Configuration.scale(110)
                    height: width
                    icon.source: SomeMediaPlayer.playing ? "img/pause_pic.png" : "img/play_pic.png"
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                BasePlayingActionsButton {
                    anchors.centerIn: parent
                    width: playPauseButton.width * 0.7
                    height: width
                    icon.source: "img/next_track_pic.png"
                }
            }
        }
    }
}
