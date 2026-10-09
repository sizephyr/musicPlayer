import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import modules.common
import modules.configuration
import modules.main_page

Button {
    id: openCurrentFolderBtn
    anchors.rightMargin: width / 2
    anchors.bottomMargin: height / 2
    width: Configuration.scale(100)
    height: width
    visible: root.model.currentPlayFolder === ""

    background: Rectangle {
        color: "transparent"
    }

    icon.source: "img/open-file.png"
    icon.height: openCurrentFolderBtn.height / 2
    icon.width: openCurrentFolderBtn.width / 2
    icon.color: Configuration.theme.darkGrayColor
}
