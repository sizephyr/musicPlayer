import QtQuick
import QtQuick.Controls
import modules.configuration

Button {
    id: root

    display: AbstractButton.IconOnly
    padding: 0

    icon.width: width * 0.8
    icon.height: height * 0.8
    icon.color: Configuration.theme.darkGrayColor

    background: Rectangle {
        color: Configuration.theme.semiTransparentGrayColor
        radius: root.height / 2
    }
}
