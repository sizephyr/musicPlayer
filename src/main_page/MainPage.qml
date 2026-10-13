import QtQuick
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page

BaseActionBackgroundRect {
    id: root
    clip: true

    Component.onCompleted: {
        model.setPlayer(SomeMediaPlayer);
    }

    property MainPageModel model: MainPageModel {}

    RowLayout {
        anchors.centerIn: parent
        width: Configuration.referenceWindowWidth()
        height: Configuration.referenceWindowHeight()
        spacing: 250

        PlayContext {
            id: playContext
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            model: root.model.playContextModel
        }
        PlayList {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

            model: root.model

            OpenCurrentFolderBtn {
                id: openCurrentFolderBtn
                anchors.fill: parent
                onClicked: activeDirectoryDialog.show()
            }
        }
    }

    ActiveDirectoryDialog {
        id: activeDirectoryDialog

        onCurrentPlayFolderChanged: folder => {
            root.model.currentPlayFolder = folder;
        }
    }
}
