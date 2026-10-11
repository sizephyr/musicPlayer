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
        anchors.rightMargin: 0

        ColumnLayout {
            anchors.fill: parent
            spacing: Configuration.scale(30)

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width

                PlayContextCover {
                    id: cover
                    anchors.fill: parent
                    anchors.margins: Configuration.scale(20)
                    border.width: Configuration.scale(6)
                    border.color: Configuration.theme.semiTransparentMutedWhiteColor
                    source: root.model.coverSource
                    onSourceChanged: console.debug(source)

                    PlayContextBasePlayingActions {
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        visible: (root?.model?.contextCompletted && hoverHandler.hovered) ?? false
                        radiusCorrecting: parent.border.width
                        onPlayNext: root.model.playNext()
                        onPlayPrevious: root.model.playPrevious()
                    }
                }
            }

            PlayProgressBar {
                id: playProgressBar
                Layout.fillWidth: true
            }
        }
    }

    HoverHandler {
        id: hoverHandler
    }
}
