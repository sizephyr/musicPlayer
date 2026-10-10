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
            if (coverPathString === "") {
                playContext.coverSource = "";
            } else {
                playContext.coverSource = "file://" + coverPathString;
            }
        }
        onCurrentPlayFolderChanged: {
            if (root.model.currentPlayFolder === "") {
                playContext.playContextAvailable = false;
            }
        }
    }
    property SomeMediaPlayer mediaPlayer: SomeMediaPlayer

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
                let trackPath = root.model.trackPathOf(uid);
                if (trackPath !== "") {
                    root.mediaPlayer.source = "file://" + trackPath;
                    root.model.provideCoverFor(uid);
                    root.mediaPlayer.play();
                } else {
                    playContext.coverSource = "";
                    root.mediaPlayer.stop();
                }
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
            playContext.playContextAvailable = true;
            root.model.currentPlayFolder = folder;
        }
    }
}
