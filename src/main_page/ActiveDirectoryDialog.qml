import QtCore
import QtQuick
import QtQuick.Dialogs

Item {
    id: root

    signal currentPlayFolderChanged(url folder)
    function show() {
        activeDirectoryDialog.active = true;
    }

    onCurrentPlayFolderChanged: {
        activeDirectoryDialog.active = false;
    }

    Loader {
        id: activeDirectoryDialog
        active: false

        sourceComponent: Component {
            FolderDialog {
                id: folderDialog
                title: "Выберите папку с медиа"
                currentFolder: StandardPaths.writableLocation(StandardPaths.HomeLocation)

                onAccepted: {
                    console.log("Выбранная папка (URL):", folderDialog.selectedFolder);
                    root.currentPlayFolderChanged(selectedFolder);
                }

                onRejected: {
                    console.log("Выбор отменен");
                    activeDirectoryDialog.active = false;
                }

                Component.onCompleted: {
                    open();
                }
            }
        }
    }
}
