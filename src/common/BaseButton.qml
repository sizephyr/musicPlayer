import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration
import modules.common

Button {
    id: root

    property int style: BaseButtonStyle.Main
    property alias border: backgroundRect.border
    property alias radius: backgroundRect.radius

    property color color100: {
        if (style === BaseButtonStyle.Main)
            return Configuration.theme.lightGreenColor;
        else if (style === BaseButtonStyle.RedMain)
            return Configuration.theme.lightRedColor;
        else if (style === BaseButtonStyle.Alternative)
            return Configuration.theme.lightGreenColor;
        else if (style === BaseButtonStyle.Red)
            return Configuration.theme.lightRedColor;
        else
            return Configuration.theme.greenColor;
    }
    property color color200: {
        if (style === BaseButtonStyle.Main)
            return Configuration.theme.greenColor;
        else if (style === BaseButtonStyle.RedMain)
            return Configuration.theme.whiteColor;
        else if (style === BaseButtonStyle.Alternative)
            return Configuration.theme.whiteColor;
        else if (style === BaseButtonStyle.Red)
            return Configuration.theme.redColor;
        else
            return Configuration.theme.lightGreenColor;
    }
    property color textColor: {
        if (style === BaseButtonStyle.Main)
            return Configuration.theme.whiteColor;
        else if (style === BaseButtonStyle.RedMain)
            return Configuration.theme.redColor;
        else if (style === BaseButtonStyle.Alternative)
            return Configuration.theme.greenColor;
        else if (style === BaseButtonStyle.Red)
            return Configuration.theme.whiteColor;
        else
            return Configuration.theme.greenColor;
    }

    display: AbstractButton.TextBesideIcon
    icon.width: font.pointSize
    icon.height: font.pointSize
    palette.buttonText: root.down ? root.color200 : root.textColor
    font.bold: true
    font.pointSize: Configuration.regularTextPointSize
    font.family: Configuration.baseFontFamily
    background: Rectangle {
        id: backgroundRect
        color: {
            if (!root.enabled)
                Configuration.theme.lightGrayColor;
            else
                root.down ? root.color100 : root.color200;
        }
        radius: Configuration.r4Radius
        border.color: {
            if (!root.enabled) {
                Configuration.theme.grayColor;
            } else {
                if (style === BaseButtonStyle.Alternative)
                    Configuration.theme.greenColor;
                else if (style === BaseButtonStyle.RedMain)
                    Configuration.theme.redColor;
                else
                    Configuration.theme.grayColor;
            }
        }
        border.width: {
            if (style === BaseButtonStyle.Alternative)
                Configuration.scale(3);
            else if (style === BaseButtonStyle.RedMain)
                1;
            else
                0;
        }
    }
}
