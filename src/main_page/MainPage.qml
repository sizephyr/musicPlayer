import QtQuick
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page
import QtMultimedia

BaseActionBackgroundRect {
    id: root
    clip: true

    property MainPageModel model: MainPageModel {
        onCoverProvided: coverPathString => {
            playContext.coverSource = "file://" + coverPathString;
        }
    }
    property SomeMediaPlayer mediaPlayer: SomeMediaPlayer {}

    RowLayout {
        anchors.fill: parent
        spacing: Configuration.scale(100)

        PlayContext {
            id: playContext
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
                root.model.provideCoverFor(uid);
                root.mediaPlayer.play();
            }

            OpenCurrentFolderBtn {
                id: openCurrentFolderBtn
                anchors.centerIn: parent
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
