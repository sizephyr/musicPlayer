import QtQuick
import QtQuick.Controls.Basic
import modules.configuration
import modules.settings_page
import modules.page_manager
import modules.core

Window {
    id: root
    width: Configuration.displaySize.width
    height: Configuration.displaySize.height
    minimumWidth: Configuration.referenceWindowWidth
    minimumHeight: Configuration.referenceWindowHeight
    flags: Configuration.windowFlags

    visible: true
    color: "transparent"

    property bool closeWindow: false
    property Core core: Core {
        Component.onCompleted: {
            setPageManager(PageManager);
        }
    }

    Component.onCompleted: {
        root.flags = Configuration.windowFlags;
    }

    Binding {
        target: Configuration
        property: "displaySize"
        value: Qt.size(root.width, root.height)
    }

    BackgroundImage {
        anchors.fill: parent
    }

    Loader {
        id: contentLoader
        anchors.fill: parent
        source: PageManager.currentPageContextModel //
        ? PageManager.currentPageContextModel.pageSource : ""
    }

    SettingsPage {
        id: settingsPage

        width: Configuration.referenceWindowWidth * 0.8
        height: Configuration.referenceWindowHeight * 0.8
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
    }

    Button {
        id: settingsButton

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 40
        height: 30
        text: "⚙"
        font.pointSize: Configuration.regularTextPointSizeStatic
        palette.buttonText: Configuration.theme.lightGrayColor

        background: Rectangle {
            bottomLeftRadius: !!(Configuration.windowFlags & Qt.FramelessWindowHint) //
            ? Configuration.r5RadiusStatic //
            : 0
            topRightRadius: Configuration.r5RadiusStatic
            color: Configuration.theme.semiTransparentLightGrayColor
        }

        onClicked: settingsPage.open()
    }

    Label {
        id: version

        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        color: Configuration.theme.semiTransparentWhiteColor
        text: Configuration.applicationVersion
    }
}
