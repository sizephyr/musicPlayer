import QtQuick
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page

BaseActionBackgroundRect {
    id: root

    Component.onCompleted: {
        model.setPlayer(SomeMediaPlayer);
    }

    property MainPageModel model: MainPageModel {
        playContextModel.contextCompletted: playListItem.count !== 0
    }

    RowLayout {
        anchors.centerIn: parent
        width: Configuration.referenceWindowWidth
        height: Configuration.referenceWindowHeight
        spacing: 250

        PlayContext {
            id: playContext
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            model: root.model.playContextModel
        }
        PlayList {
            id: playListItem
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter

            model: root.model

            onAppendFileCalled: activeDirectoryDialog.selectFile()
            onAppendFolderCalled: activeDirectoryDialog.selectFolder()
        }
    }

    ActiveDirectoryDialog {
        id: activeDirectoryDialog

        onFolderSelected: folder => root.model.onFolderSelected(folder)
        onFileSelected: file => root.model.onFileSelected(file)
    }
}
