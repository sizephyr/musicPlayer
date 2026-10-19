import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration
import modules.main_page

Item {
    id: root

    clip: true
    visible: width > 0

    readonly property int baseButtonHeight: 40

    signal appendFileCalled
    signal appendFolderCalled

    ColumnLayout {
        anchors.fill: parent
        anchors.rightMargin: 10

        PlayListActionsButton {
            Layout.fillWidth: true
            Layout.preferredHeight: root.baseButtonHeight
            icon.source: "img/append_file.png"
            onClicked: root.appendFileCalled()
        }
        PlayListActionsButton {
            Layout.fillWidth: true
            Layout.preferredHeight: root.baseButtonHeight
            icon.source: "img/append_folder.png"
            onClicked: root.appendFolderCalled()
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: Configuration.animationDurationMs
            easing.type: Easing.InOutQuad
        }
    }
}
