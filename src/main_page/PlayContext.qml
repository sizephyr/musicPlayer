import QtQuick
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    property PlayContextModel model

    Rectangle {
        anchors.centerIn: parent
        width: 500
        height: 500
        // color: Configuration.theme.semiTransparentGrayColor
        color: "transparent"

        ColumnLayout {
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.75
            height: width

            spacing: Configuration.scale(0)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width

                PlayContextCover {
                    id: cover
                    source: root.model.coverSource
                    anchors.fill: parent
                    anchors.margins: 30

                    // след/пред трек, плей/пауза
                    PlayContextBasePlayingActions {
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        visible: (root?.model?.contextCompletted && hoverHandler.hovered) ?? false
                        radiusCorrecting: parent.border.width
                        onPlayNextClicked: root.model.playNext()
                        onPlayPreviousClicked: root.model.playPrevious()
                        onPlayPauseClicked: SomeMediaPlayer.playing ? SomeMediaPlayer.pause() : SomeMediaPlayer.play()
                    }
                }

                HoverHandler {
                    id: hoverHandler
                }
            }

            PlayProgressBar {
                id: playProgressBar
                Layout.fillWidth: true
                // Layout.leftMargin: Configuration.scale(50)
            }
        }
    }
}
