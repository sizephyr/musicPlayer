import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page

BaseActionBackgroundRect {
    id: root
    clip: true

    Component.onCompleted: {
        activeDirectoryDialog.show();
    }

    property MainPageModel model: MainPageModel {}

    RowLayout {
        anchors.fill: parent
        spacing: Configuration.scale(100)

        PlayContext {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        PlayList {
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: root.model.playListModel
        }
    }

    ActiveDirectoryDialog {
        id: activeDirectoryDialog

        onCurrentPlayFolderChanged: folder => {
            root.model.currentPlayFolder = folder;
        }
    }
}
