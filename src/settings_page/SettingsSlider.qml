import QtQuick
import modules.configuration
import modules.common

SettingsItem {
    id: root

    property alias value: slider.value

    BaseSlider {
        id: slider
        anchors.fill: parent
        anchors.margins: 10
        radius: Configuration.r5RadiusStatic
        orientation: BaseSlider.Orientation.LeftToRight

        Connections {
            target: slider
        }
    }
}
