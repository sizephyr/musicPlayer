import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.common
import modules.configuration
import modules.notify_manager

Button {
    id: root

    readonly property color shadowColor: "#eceef4"
    readonly property color baseYellow: "#fdb744"
    readonly property color baseGreen: "#2f8e97"
    readonly property color baseBlack: "#39414a"
    readonly property color baseBlue: "#4a76a8"
    readonly property color baseRed: "#fe6f71"

    property bool autoHide: true
    property int visibleInterval: Configuration.notificationHideAfterMs()
    property int rightMargin: 20
    property alias title: title.text
    property alias message: message.text
    property int type: NotifyManagerNotifiesViewModel.Info

    height: rootLayout.implicitHeight * 1.2 + rootLayout.anchors.margins * 2// (сверху и снизу)

    opacity: 0
    enabled: true

    onClicked: hide()

    Rectangle {
        id: closeButtonImitation

        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width * 0.15
        height: parent.height * 0.15
        color: backgroundRect.border.color
        visible: root.hovered
        topRightRadius: parent.background.radius
        bottomLeftRadius: topRightRadius

        Text {
            text: "X"
            color: "white"
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
        }
    }

    background: Rectangle {
        id: backgroundRect
        border.width: 1
        border.color: {
            if (type === NotifyManagerNotifiesViewModel.Info)
                return baseGreen;
            else if (type === NotifyManagerNotifiesViewModel.Warning)
                return baseYellow;
            else if (type === NotifyManagerNotifiesViewModel.Error)
                return baseRed;
        }

        radius: Configuration.r3Radius
    }

    ColumnLayout {
        id: rootLayout
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: backgroundRect.radius
        clip: true

        Label {
            id: title
            Layout.fillWidth: true
            font.bold: true
            font.pointSize: Configuration.h4PointSize
            color: root.baseBlack
        }
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 2
            color: root.shadowColor
        }
        Label {
            id: message
            Layout.fillWidth: true
            font.pointSize: Configuration.regularTextPointSize
            color: root.baseBlack
            wrapMode: Text.WordWrap
        }
        NotifyLastTimeIndicator {
            id: timerIndicator
            Layout.fillWidth: true
            Layout.preferredHeight: 3
            visible: root.autoHide
            color: root.baseBlue
            visibleInterval: root.visibleInterval
        }
    }

    transform: [
        Translate {
            id: yTranslate
            y: -root.height
        },
        Scale {
            id: scaleTransform
            origin.x: root.width / 2
            origin.y: root.height / 2
        }
    ]

    function hide() {
        hideAnimation.start();
    }

    Component.onCompleted: showAnimation.start()

    ParallelAnimation {
        id: showAnimation

        NumberAnimation {
            target: root
            property: "opacity"
            to: 1.0
            duration: Configuration.animationDurationMs()
            easing.type: Easing.OutCubic
        }
        NumberAnimation {
            target: yTranslate
            property: "y"
            to: 0
            duration: Configuration.animationDurationMs()
            easing.type: Easing.OutCubic
        }
    }

    ParallelAnimation {
        id: hideAnimation

        NumberAnimation {
            target: root
            property: "opacity"
            to: 0.0
            duration: Configuration.animationDurationMs()
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: scaleTransform
            property: "xScale"
            to: 0.85
            duration: Configuration.animationDurationMs()
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: scaleTransform
            property: "yScale"
            to: 0.85
            duration: Configuration.animationDurationMs()
            easing.type: Easing.InOutQuad
        }

        onFinished: root.visible = false
    }

    Timer {
        id: hideTimer

        interval: root.visibleInterval
        running: root.autoHide
        onTriggered: root.hide()
    }
}
