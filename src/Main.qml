import QtQuick
import QtQuick.Effects
import modules.configuration
import modules.notify_manager
import modules.page_manager
import modules.core

Window {
    id: root
    objectName: "mainWindow"
    width: Configuration.displaySize.width
    height: Configuration.displaySize.height
    minimumWidth: Configuration.referenceWindowWidth()
    minimumHeight: Configuration.referenceWindowHeight()

    visible: true

    property bool closeWindow: false

    readonly property double aspectRatio: 16.0 / 9.0

    // Флаг для предотвращения бесконечной рекурсии сигналов
    property bool isAdjusting: false

    Component.onCompleted: {
        root.flags = core.windowFlags;
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

            visible: false
        }

        Rectangle {
            id: backgroundImageMaskRectangle
            anchors.fill: backgroundImage
            radius: Configuration.r5Radius
            color: "black"
            visible: false
            layer.enabled: true
        }

        MultiEffect {
            anchors.fill: backgroundImage
            source: backgroundImage
            maskEnabled: true
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

    onWidthChanged: {
        if (!isAdjusting) {
            isAdjusting = true;
            // Корректируем высоту под ширину
            root.height = Math.round(root.width / aspectRatio);
            isAdjusting = false;
        }
    }

    onHeightChanged: {
        if (!isAdjusting) {
            isAdjusting = true;
            // Корректируем ширину под высоту
            root.width = Math.round(root.height * aspectRatio);
            isAdjusting = false;
        }
    }
}
