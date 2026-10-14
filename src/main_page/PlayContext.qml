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
                        radiusCorrecting: parent.border.width

                        model: root.model
                        canShow: (root?.model?.contextCompletted && hoverHandler.hovered) ?? false
                    }
                }

                VolumeControl {
                    anchors.top: parent.top
                    anchors.right: cover.left
                    anchors.bottom: parent.bottom
                    anchors.topMargin: 100 + cover.anchors.margins
                    anchors.bottomMargin: cover.anchors.margins + 5
                    anchors.rightMargin: 5
                    height: cover.height
                    width: 25
                    canShow: hoverHandler.hovered
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
