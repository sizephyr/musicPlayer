import QtQuick
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    property int radiusCorrecting

    signal playPauseClicked
    signal playNextClicked
    signal playPreviousClicked

    Rectangle {
        anchors.fill: parent

        radius: Math.max(0, Configuration.r3RadiusStatic - root.radiusCorrecting)
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
                    onClicked: root.playPreviousClicked()
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                BasePlayingActionsButton {
                    id: playPauseButton
                    anchors.centerIn: parent
                    width: 110
                    height: width
                    icon.source: SomeMediaPlayer.playing ? "img/pause_pic.png" : "img/play_pic.png"

                    onClicked: root.playPauseClicked()
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
                    onClicked: root.playNextClicked()
                }
            }
        }
    }
}
