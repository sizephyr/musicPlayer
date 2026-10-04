import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import modules.page_manager
import modules.configuration
import modules.header

Rectangle {
    id: root
    bottomLeftRadius: Configuration.r1Radius
    bottomRightRadius: Configuration.r1Radius
    color: "white"
    border.color: mainColor
    border.width: Configuration.scale(4)
    anchors.topMargin: -border.width
    clip: true
    visible: PageManager.currentPageContextModel?.showHeader ?? false

    property LoggingCategory loggingCategory: LoggingCategory {
        name: "header.ui"
    }

    property var callBeforeClose: HeaderModel.callBeforeClose
    property color mainColor: Configuration.theme.greenColor
    property bool serverAvailable: false

    signal menuTriggered
    signal showServerState

    onVisibleChanged: {
        if (visible)
            show();
    }

    RowLayout {
        id: base
        anchors.fill: parent
        anchors.margins: Configuration.scale(10)

        Button {
            Layout.preferredWidth: Configuration.scale(50)
            Layout.fillHeight: true
            enabled: PageManager.thereIsPreviousPage
            text: PageManager.thereIsPreviousPage ? "<" : " "
            font.pointSize: Configuration.h1PointSize
            palette.buttonText: root.mainColor
            background: Rectangle {
                color: "transparent"
            }
            onClicked: {
                if (HeaderModel.callBeforeClose) {
                    HeaderModel.callBeforeClose();
                } else {
                    PageManager.goBack();
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ApplicationNameLabel {
            color: root.mainColor
            Layout.fillWidth: true
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            id: menuButton
            Layout.preferredWidth: Configuration.scale(80)
            Layout.fillHeight: true
            Layout.rightMargin: Configuration.scale(20)

            background: Rectangle {
                color: "transparent"
            }
            text: "⚙"
            font.pointSize: Configuration.h1PointSize
            palette.buttonText: root.serverAvailable ? Configuration.theme.greenColor : Configuration.theme.redColor

            onClicked: {
                menuButton.rotation = menuButton.rotation + 90;
                root.menuTriggered();
            }

            Behavior on rotation {
                NumberAnimation {
                    duration: Configuration.animationDurationMs()
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    NumberAnimation {
        id: appearanceAnimation
        target: translateTransform
        property: "y"
        from: root.height
        to: 0
        duration: Configuration.animationDurationMs()
        easing.type: Easing.OutCubic

        onStarted: {
            if (appearanceAnimation.to === 0)
                visible = true;
        }
        onFinished: {
            if (translateTransform.to === root.height)
                visible = false;
        }
    }

    transform: Translate {
        id: translateTransform
    }
    Connections {
        target: HeaderModel

        function onCallBeforeCloseChanged() {
            if (HeaderModel.callBeforeClose)
                console.debug(loggingCategory, "Перехват события выхода со страницы");
        }

        function onCanBeClosed() {
            HeaderModel.callBeforeClose = null;
            PageManager.goBack();
        }
    }

    function show() {
        appearanceAnimation.from = -root.height;
        appearanceAnimation.to = 0;
        appearanceAnimation.restart();
    }

    function hide() {
        appearanceAnimation.from = 0;
        appearanceAnimation.to = -root.height;
        appearanceAnimation.restart();
    }
}
