import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Button {
    id: root

    icon.color: Configuration.theme.mutedWhiteColor
    background: Rectangle {
        topRightRadius: Configuration.r4RadiusStatic
        bottomRightRadius: Configuration.r4RadiusStatic
        color: Configuration.theme.semiTransparentLightGrayColor
        border.width: 1
        border.color: Configuration.theme.extrapolateAlpha(Configuration.theme.mutedWhiteColor, 0.4)
    }
}
