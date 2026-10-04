import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import modules.configuration
import modules.header

Label {
    id: root
    font.pointSize: Configuration.h2PointSize
    color: Configuration.theme.redColor

    property int animationDuration: 300
    property int delayInterval

    function show(notifyText) {
        root.text = notifyText;
        freeSpaceWaitingTimer.restart();
    }
    function hide() {
        hideAnimation.restart();
    }

    Timer {
        id: freeSpaceWaitingTimer
        interval: root.delayInterval
        onTriggered: showAnimation.restart()
    }

    SequentialAnimation {
        id: showAnimation

        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "y"
                from: -root.y
                to: root.y
                duration: root.animationDuration
                easing.type: Easing.OutCubic
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: root.animationDuration
                easing.type: Easing.OutCubic
            }
        }
        onStarted: root.visible = true
    }

    SequentialAnimation {
        id: hideAnimation

        ParallelAnimation {
            // NumberAnimation {
            //     target: root
            //     property: "y"
            //     from: root.y
            //     to: root.y * 2
            //     duration: root.animationDuration
            //     easing.type: Easing.InCubic
            // }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 1.0
                to: 0.0
                duration: root.animationDuration
                easing.type: Easing.InCubic
            }
        }
        onFinished: {
            root.text = "";
            root.visible = false;
        }
    }
}
