import QtQuick
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    property PlayContextModel model

    Item {
        anchors.fill: parent
        anchors.margins: Configuration.scale(100)
        anchors.topMargin: Configuration.scale(40)
        anchors.rightMargin: 0

        ColumnLayout {
            anchors.fill: parent
            anchors.bottomMargin: Configuration.scale(25)
            spacing: Configuration.scale(0)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width

                PlayContextCover {
                    id: cover
                    anchors.fill: parent
                    anchors.margins: Configuration.scale(20)
                    anchors.bottomMargin: Configuration.scale(0)
                    // компенсируем ноль снизу для сохранения квадратности
                    anchors.topMargin: anchors.margins * 2
                    source: root.model.coverSource

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
            }
        }
    }
}
