import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import modules.common
import modules.configuration
import modules.page_manager

BaseActionBackgroundRect {
    id: root
    clip: true

    RowLayout {
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Item {
                anchors.fill: parent
                anchors.margins: 100

                ColumnLayout {
                    anchors.fill: parent
                    ShadowedRectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        selected: true
                        baseColor: Configuration.theme.lightGrayColor
                    }
                    ShadowedRectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 3

                        selected: true
                        baseColor: Configuration.theme.lightGrayColor
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
