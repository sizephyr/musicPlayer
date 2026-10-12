import QtQuick
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    property PlayContextModel model

    Item {
        anchors.fill: parent
        anchors.bottomMargin: Configuration.scale(100)
        anchors.topMargin: Configuration.scale(40)
        anchors.rightMargin: 0
        anchors.leftMargin: Configuration.scale(50)

        ColumnLayout {
            anchors.fill: parent
            anchors.bottomMargin: Configuration.scale(25)
            spacing: Configuration.scale(0)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width

                PlayContextCover {
                    id: cover
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: Configuration.scale(15)
                    anchors.bottomMargin: Configuration.scale(5)
                    width: parent.width * 0.8
                    height: width

                    source: root.model.coverSource

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
                Layout.leftMargin: Configuration.scale(50)
            }
        }
    }
}
