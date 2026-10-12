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
        anchors.fill: parent
        spacing: Configuration.scale(100)

        PlayContext {
            id: playContext
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: root.model.playContextModel
        }
        PlayList {
            Layout.fillHeight: true
            Layout.fillWidth: true

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
