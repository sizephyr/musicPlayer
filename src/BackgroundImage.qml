import QtQuick
import QtQuick.Effects
import modules.configuration

Item {
    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "file:" + Configuration.windowBackground()

        visible: !(Configuration.windowFlags & Qt.FramelessWindowHint)
    }

    Rectangle {
        id: backgroundImageMaskRectangle
        anchors.fill: backgroundImage
        radius: !!(Configuration.windowFlags & Qt.FramelessWindowHint) //
        ? Configuration.r5RadiusStatic //
        : 0
        color: "black"
        visible: false
        layer.enabled: !!(Configuration.windowFlags & Qt.FramelessWindowHint)
    }

    MultiEffect {
        anchors.fill: backgroundImage
        source: backgroundImage
        maskEnabled: !!(Configuration.windowFlags & Qt.FramelessWindowHint)
        maskSource: backgroundImageMaskRectangle
    }
}
