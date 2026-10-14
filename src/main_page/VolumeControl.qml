import QtQuick
import modules.configuration
import modules.main_page

Rectangle {
    id: root

    border.width: 1
    border.color: Configuration.theme.semiTransparentLightGrayColor
    radius: Configuration.r3RadiusStatic
    opacity: canShow ? 1.0 : 0.0

    readonly property double progressBegin: 0.0
    property bool canShow: true
    property double progress: SomeMediaPlayer.audioOutput.volume

    gradient: Gradient {
        orientation: Gradient.Vertical

        GradientStop {
            id: notListenedGS
            position: 0.0
            color: Configuration.theme.lightGrayColor
        }
        GradientStop {
            id: notListenedBoundGS
            property double width: root.calculateBoundWidth() / 2
            property double calculatedPosition: root.progress + listenedBoundGS.width + width

            position: Math.max(0.0, Math.min(1.0, 1.0 - calculatedPosition))
            color: Configuration.theme.lightGrayColor
        }
        GradientStop {
            id: listenedBoundGS
            property double width: root.calculateBoundWidth() / 2
            position: Math.max(0.0, Math.min(1.0, 1.0 - (root.progress + width)))
            color: Configuration.theme.grayColor
        }
        GradientStop {
            id: listenedGS
            position: Math.max(0.0, Math.min(1.0, 1.0 - root.progress))
            color: Configuration.theme.darkGrayColor
        }
        GradientStop {
            position: 1.0
            color: Configuration.theme.darkGrayColor
        }
    }

    // изменение громкости нажатием и перетаскиванием
    PointHandler {
        id: trackHandler
        target: null

        onPointChanged: {
            if (trackHandler.active) {
                let currentY = trackHandler.point.position.y;

                SomeMediaPlayer.audioOutput.volume = root.getProgressRatio(root.height - currentY);
            }
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: Configuration.animationDurationMs
            easing.type: Easing.InOutQuad
        }
    }

    function calculateBoundWidth() {
        const p = Math.max(0.0, Math.min(1.0, progress));

        const leftProgressControlPoint = 0.2;
        const rightProgressControlPoint = 0.9;
        const targetMax = 0.1;

        if (p <= leftProgressControlPoint) {
            return (targetMax / leftProgressControlPoint) * p;
        } else if (p < rightProgressControlPoint) {
            return targetMax;
        } else {
            return (targetMax / (1.0 - rightProgressControlPoint)) * (1.0 - p);
        }
    }

    function getProgressRatio(position) {
        if (root.height <= 0) {
            return 0.000;
        }
        let ratio = position / root.height;

        return Math.max(0.000, Math.min(1.000, ratio));
    }
}
