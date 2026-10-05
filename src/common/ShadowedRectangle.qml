import QtQuick
import modules.configuration

Rectangle {
    id: root

    radius: Configuration.r3Radius
    default property alias content: contentItem.data

    property alias borderColor: root.color
    property alias baseColor: contentItem.color
    property bool lowProfile: false // маленькая "тень" снизу
    property bool selected: false
    property color contentColor: Configuration.theme.mutedWhiteColor

    color: selected ? Configuration.theme.grayColor : Configuration.theme.lightGrayColor

    Rectangle {
        id: contentItem
        anchors.fill: parent
        anchors.margins: root.lowProfile ? 1 : 2
        anchors.bottomMargin: root.lowProfile ? 2 : 5
        radius: root.radius
        color: root.contentColor
    }
}
