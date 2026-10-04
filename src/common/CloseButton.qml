import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Button {
    id: root

    property int baseRadius
    property alias bottomLeftRadius: backgroundRect.bottomLeftRadius
    property alias topRightRadius: backgroundRect.topRightRadius
    property alias backgroundColor: backgroundRect.color

    anchors.top: parent.top
    anchors.right: parent.right
    background: Rectangle {
        id: backgroundRect

        bottomLeftRadius: root.baseRadius
        topRightRadius: root.baseRadius
        color: root.down ? Configuration.theme.darkRedColor : Configuration.theme.redColor
    }
    text: "X"
    font.pointSize: 20
    font.bold: true
    palette.buttonText: Configuration.theme.whiteColor
}
