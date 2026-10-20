import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Label {
    id: root

    property bool selected: false
    property bool editState: false
    property int horizontalPaddings

    signal select
    signal remove

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

    Button {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: height
        background: Rectangle {
            color: "transparent"
            radius: Configuration.r5RadiusStatic
        }
        onClicked: root.remove()
        text: "×"
        palette.buttonText: Configuration.theme.redColor
        visible: root.editState
    }
}
