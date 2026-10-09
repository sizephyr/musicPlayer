import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import modules.configuration
import modules.main_page

Item {
    id: root

    property alias progress: playProgressBar.progress
    property alias coverSource: cover.source

    onCoverSourceChanged: console.info("Загружена обложка: ", coverSource)

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

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: Configuration.scale(20)

                    border.width: Configuration.scale(6)
                    border.color: Configuration.theme.semiTransparentMutedWhiteColor
                    color: Configuration.theme.semiTransparentLightGrayColor
                    radius: Configuration.r3Radius
                    clip: true

                    Image {
                        id: cover
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        visible: false
                    }

                    Rectangle {
                        id: maskRectangle
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        radius: Configuration.r3Radius
                        visible: false
                    }

                    MultiEffect {
                        anchors.fill: cover
                        anchors.margins: parent.border.width
                        source: cover
                        maskEnabled: true
                        maskSource: cover
                    }
                }
            }

            PlayProgressBar {
                id: playProgressBar
                Layout.fillWidth: true
            }
        }
    }
}
