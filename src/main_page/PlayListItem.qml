import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Label {
    id: root

    property bool selected: false
    property int horizontalPaddings

    signal select

    verticalAlignment: Text.AlignVCenter
    font.pointSize: Configuration.regularTextPointSizeStatic
    color: Configuration.theme.darkGrayColor
    font.bold: root.selected
    leftPadding: horizontalPaddings
    rightPadding: horizontalPaddings
    elide: Text.ElideRight

    background: Rectangle {
        border.width: 1
        border.color: root.selected ? Configuration.theme.darkGrayColor : Configuration.theme.semiTransparentLightGrayColor
        color: "transparent"
        radius: Configuration.r5RadiusStatic
    }

    Button {
        anchors.fill: parent
        background: Rectangle {
            color: "transparent"
            radius: Configuration.r5RadiusStatic
        }
        onClicked: root.select()
    }
}
