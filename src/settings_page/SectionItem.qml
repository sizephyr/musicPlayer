import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Label {
    id: root

    property bool selected: false

    signal select

    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    font.pointSize: Configuration.regularTextPointSizeStatic
    color: Configuration.theme.lightGrayColor
    font.bold: root.selected
    elide: Text.ElideRight

    background: Rectangle {
        border.width: 1
        border.color: root.selected ? Configuration.theme.darkGrayColor : Configuration.theme.semiTransparentLightGrayColor
        color: "transparent"
        radius: Configuration.r6RadiusStatic
        topLeftRadius: Configuration.r4RadiusStatic
        bottomLeftRadius: Configuration.r4RadiusStatic
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
