import QtQuick
import QtQuick.Controls
import modules.configuration

Button {
    id: root

    display: AbstractButton.IconOnly
    padding: 0

    icon.width: width * 0.8
    icon.height: height * 0.8
    icon.color: root.enabled ? Configuration.theme.darkGrayColor : Configuration.theme.semiTransparentDarkGrayColor

    background: Rectangle {
        color: "transparent"
        radius: root.height / 2
    }
}
