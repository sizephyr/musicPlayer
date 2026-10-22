import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration

Item {
    id: root

    property alias text: settingsName.text
    default property alias content: contentItem.data

    property alias clipText: textItem.clip

    RowLayout {
        anchors.fill: parent

        Item {
            id: textItem
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 15
            clip: true

            Label {
                id: settingsName
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                color: Configuration.theme.lightGrayColor
            }
        }
        Item {
            id: contentItem
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 30
        }
    }
}
