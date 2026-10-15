import QtQuick
import QtQuick.Effects
import modules.configuration
import modules.notify_manager
import modules.page_manager
import modules.core

Window {
    id: root
    width: Configuration.displaySize.width
    height: Configuration.displaySize.height
    minimumWidth: Configuration.referenceWindowWidth()
    minimumHeight: Configuration.referenceWindowHeight()
    flags: Configuration.windowFlags

    visible: true
    color: "transparent"

    property bool closeWindow: false

    Component.onCompleted: {
        root.flags = Configuration.windowFlags;
    }
    Binding {
        target: Configuration
        property: "displaySize"
        value: Qt.size(width, height)
    }

    Core {
        id: core
        anchors.fill: parent

        Component.onCompleted: {
            setPageManager(PageManager);
        }

        Image {
            id: backgroundImage
            anchors.fill: parent
            source: "file:" + Configuration.windowBackground()

            visible: !(root.flags & Qt.FramelessWindowHint)
        }

        Rectangle {
            id: backgroundImageMaskRectangle
            anchors.fill: backgroundImage
            radius: !!(Configuration.windowFlags & Qt.FramelessWindowHint) //
            ? Configuration.r5RadiusStatic //
            : 0
            color: "black"
            visible: false
            layer.enabled: root.flags & Qt.FramelessWindowHint
        }

        MultiEffect {
            anchors.fill: backgroundImage
            source: backgroundImage
            maskEnabled: !!(Configuration.windowFlags & Qt.FramelessWindowHint)
            maskSource: backgroundImageMaskRectangle
        }

        Item {
            id: contentItem
            anchors.fill: parent

            Loader {
                id: contentLoader
                anchors.fill: parent
                source: PageManager.currentPageContextModel ? PageManager.currentPageContextModel.pageSource : ""
            }
        }
    }

    NotifyManager {
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }

    Connections {
        target: PageManager

        function onLastPageClosed() {
            root.close();
        }
    }
}
