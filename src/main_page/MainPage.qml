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
                    spacing: 50

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: width

                        border.width: 4
                        border.color: Configuration.theme.semiTransparentMutedWhiteColor
                        color: Configuration.theme.semiTransparentLightGrayColor
                        radius: Configuration.r3Radius
                    }

                    PlayProgressBar {
                        id: playProgressBar
                        Layout.fillWidth: true
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
