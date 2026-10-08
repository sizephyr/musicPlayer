import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts
import QtMultimedia
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

            onSelected: uid => {
                mediaPlayer.source = "file://" + root.model.trackPathOf(uid);
                mediaPlayer.play();
            }
        }
    }

    MediaPlayer {
        id: mediaPlayer
        // source: "file:///path/to/your/audio.mp3"

        audioOutput: AudioOutput {
            // TODO: привязать значение из ProgressBar
            // volume: volumeSlider.value
            volume: 0.5
        }

        onErrorOccurred: (error, errorString) => {
            console.log("Ошибка воспроизведения: " + errorString);
        }
    }

    ActiveDirectoryDialog {
        id: activeDirectoryDialog

        onCurrentPlayFolderChanged: folder => {
            root.model.currentPlayFolder = folder;
        }
    }
}
