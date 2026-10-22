import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration
import modules.common

SettingsItem {
    id: root

    property alias checked: checkBox.checked
    clipText: false

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        CheckBox {
            id: checkBox

            indicator: Rectangle {
                implicitWidth: 20
                implicitHeight: 20
                x: checkBox.leftPadding
                y: parent.height / 2 - height / 2
                radius: Configuration.r6RadiusStatic
                border.color: Configuration.theme.lightGrayColor
                color: "transparent"

                // Внутренний квадрат/галочка при checked
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 4
                    radius: 2
                    color: Configuration.theme.lightGrayColor
                    visible: checkBox.checked
                }
            }
        }
    }
}
