import QtQuick
import modules.configuration
import modules.notify_manager
import modules.page_manager
import modules.core

Window {
    width: Configuration.windowWidth()
    height: Configuration.windowHeight()
    minimumWidth: Configuration.windowWidth()
    minimumHeight: Configuration.windowHeight()
    maximumWidth: Configuration.windowWidth()
    maximumHeight: Configuration.windowHeight()

    visible: true
    flags: Qt.Window

    property bool closeWindow: false

    Component.onCompleted: {
        Configuration.setDisplaySize(height, width);
    }

    Core {
        id: core
        anchors.fill: parent

        Component.onCompleted: {
            setPageManager(PageManager);
        }

        Image {
            anchors.fill: parent
            source: "file:" + Configuration.windowBackground()

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
