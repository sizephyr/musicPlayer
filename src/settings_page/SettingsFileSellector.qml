import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import modules.configuration

SettingsItem {
    id: root

    property string selectedFile
    property string currentFolder
    property string defaultSuffix

    signal accepted

    Button {
        anchors.fill: parent
        anchors.margins: 10

        background: Rectangle {
            color: Configuration.theme.semiTransparentGrayColor
            radius: Configuration.r5RadiusStatic
        }
        text: Configuration.windowBackground
        onClicked: loader.active = true
    }

    Loader {
        id: loader
        active: false
        sourceComponent: Component {
            FileDialog {
                Component.onCompleted: {
                    open();
                }

                defaultSuffix: root.defaultSuffix
                currentFolder: root.currentFolder
                onAccepted: {
                    root.selectedFile = selectedFile;
                    root.accepted();
                    loader.active = false;
                }
            }
        }
    }
}
