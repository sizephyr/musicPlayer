import QtQuick
import QtQuick.Effects
import modules.configuration

Rectangle {
    id: root

    property alias source: cover.source

    color: Configuration.theme.semiTransparentLightGrayColor
    radius: Configuration.r3RadiusStatic
    border.width: 3
    border.color: Configuration.theme.semiTransparentLightGrayColor

    Image {
        id: coverMock // заглушка с вопросительным знаком, если нет обложки
        anchors.fill: parent
        anchors.margins: 100
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
}
