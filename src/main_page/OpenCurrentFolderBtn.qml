import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page

Button {
    id: openCurrentFolderBtn

    visible: root.model.currentPlayFolder === ""

    background: Rectangle {
        color: "transparent"
    }

    icon.source: "img/open-file.png"
    icon.height: openCurrentFolderBtn.height / 6
    icon.width: openCurrentFolderBtn.width / 6
    icon.color: Configuration.theme.darkGrayColor
}
