import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import modules.configuration
import modules.main_page

Item {
    id: root

    // есть ли чё воспроизводить
    property bool playContextAvailable: false

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

                    Image {
                        id: coverMock // заглушка с вопросительным знаком, если нет обложки
                        anchors.fill: parent
                        anchors.margins: Configuration.scale(100)
                        source: "img/unknown_icon_pic.png"

                        layer.enabled: true
                        layer.effect: MultiEffect {
                            source: coverMock
                            brightness: 1.0
                            colorization: 1.0
                            colorizationColor: Configuration.theme.darkGrayColor
                        }
                    }

                    Image {
                        id: cover
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        visible: false
                    }

                    Rectangle {
                        id: coverMaskRectangle
                        anchors.fill: cover
                        radius: Math.max(0, parent.radius - parent.border.width)
                        color: "black"
                        visible: false
                        layer.enabled: true
                    }

                    MultiEffect {
                        anchors.fill: cover
                        source: cover
                        maskEnabled: true
                        maskSource: coverMaskRectangle
                    }

                    PlayContextBasePlayingActions {
                        anchors.fill: parent
                        anchors.margins: parent.border.width
                        visible: root.playContextAvailable && hoverHandler.hovered
                        radiusCorrecting: parent.border.width
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
