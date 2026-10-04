import QtQuick
import modules.configuration
import modules.notify_manager
import modules.page_manager
import modules.core
import modules.header

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
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height * 0.9
                width: parent.width * 0.95

                Loader {
                    id: contentLoader
                    anchors.fill: parent
                    source: PageManager.currentPageContextModel ? PageManager.currentPageContextModel.pageSource : ""
                }
            }

            HeaderMenu {
                id: headerMenu
                anchors.top: header.top
                anchors.right: header.right
                anchors.rightMargin: header.border.width
                baseHeight: header.height
                width: header.width - header.border.width * 2
                mainColor: header.mainColor
            }
            Header {
                id: header
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                height: parent.height * 0.085
                width: parent.width * 0.95
                clip: true
                onMenuTriggered: {
                    headerMenu.expanded = !headerMenu.expanded;
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
