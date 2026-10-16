import QtQuick
import QtQuick.Controls.Basic
import modules.configuration

Button {
    id: openCurrentFolderBtn

    background: Rectangle {
        color: "transparent"
    }

    icon.source: "img/open-file.png"
    icon.height: openCurrentFolderBtn.height / 6
    icon.width: openCurrentFolderBtn.width / 6
    icon.color: Configuration.theme.darkGrayColor
}
