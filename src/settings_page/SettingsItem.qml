import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

Item {
    id: root

    property alias text: settingsName.text
    default property alias content: contentItem.data

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 15
            clip: true

            Label {
                id: settingsName
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
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
