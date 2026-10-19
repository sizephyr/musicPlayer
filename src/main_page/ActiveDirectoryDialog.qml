import QtCore
import QtQuick
import QtQuick.Dialogs

Item {
    id: root

    signal folderSelected(url folder)
    signal fileSelected(url file)

    function selectFolder() {
        directoryDialogLoader.active = true;
    }
    function selectFile() {
        fileDialogLoader.active = true;
    }

    onFolderSelected: {
        directoryDialogLoader.active = false;
    }
    onFileSelected: {
        fileDialogLoader.active = false;
    }

    Loader {
        id: directoryDialogLoader
        active: false

        sourceComponent: Component {
            FolderDialog {
                title: "Выберите папку с медиа"
                currentFolder: StandardPaths.writableLocation(StandardPaths.HomeLocation)

                onAccepted: {
                    console.log("Выбранная папка (URL):", selectedFolder);
                    root.folderSelected(selectedFolder);
                }

                onRejected: {
                    console.log("Выбор отменен");
                    directoryDialogLoader.active = false;
                }

                Component.onCompleted: {
                    open();
                }
            }
        }
    }
    Loader {
        id: fileDialogLoader
        active: false

        sourceComponent: Component {
            FileDialog {
                title: "Выберите файл"
                currentFolder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
                defaultSuffix: "mp3"

                onAccepted: {
                    console.log("Выбранный файл (URL):", selectedFile);
                    root.fileSelected(selectedFile);
                }

                onRejected: {
                    console.log("Выбор отменен");
                    fileDialogLoader.active = false;
                }

                Component.onCompleted: {
                    open();
                }
            }
        }
    }
}
