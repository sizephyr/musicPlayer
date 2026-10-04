import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Basic
import modules.configuration
import modules.header

Label {
    id: root

    font.pointSize: Configuration.h1PointSize
    text: Configuration.windowTitle()
    font.bold: true
    horizontalAlignment: Qt.AlignHCenter

    function show() {
        showAnimation.restart();
    }
    function hide() {
        hideAnimation.restart();
    }
    SequentialAnimation {
        id: showAnimation

        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "y"
                from: -root.y
                to: root.y
                duration: Configuration.animationDurationMs()
                easing.type: Easing.OutCubic
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: Configuration.animationDurationMs()
                easing.type: Easing.OutCubic
            }
        }
    }

    SequentialAnimation {
        id: hideAnimation

        ParallelAnimation {
            NumberAnimation {
                target: root
                property: "y"
                from: root.y
                to: root.y * 2
                duration: Configuration.animationDurationMs()
                easing.type: Easing.InCubic
            }
            NumberAnimation {
                target: root
                property: "opacity"
                from: 1.0
                to: 0.0
                duration: Configuration.animationDurationMs()
                easing.type: Easing.InCubic
            }
        }
    }
}
