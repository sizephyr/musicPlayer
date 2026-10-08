import QtQuick
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
    property SomeMediaPlayer mediaPlayer: SomeMediaPlayer {}

    RowLayout {
        anchors.fill: parent
        spacing: Configuration.scale(100)

        PlayContext {
            Layout.fillHeight: true
            Layout.fillWidth: true
            progress: root.mediaPlayer.progressRatio
        }
        PlayList {
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: root.model.playListModel

            onSelected: uid => {
                root.mediaPlayer.source = "file://" + root.model.trackPathOf(uid);
                root.mediaPlayer.play();
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
