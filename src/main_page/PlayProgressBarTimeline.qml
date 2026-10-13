import QtQuick
import modules.configuration
import modules.main_page

Rectangle {
    id: root

    property bool hovered: hoverHandler.hovered

    border.width: 1
    border.color: Configuration.theme.semiTransparentLightGrayColor
    radius: Configuration.r3RadiusStatic

    readonly property double progressBegin: 0.0
    property double progress: SomeMediaPlayer.progressRatio

    gradient: Gradient {
        orientation: Gradient.Horizontal

        GradientStop {
            position: root.progressBegin
            color: Configuration.theme.darkGrayColor
        }
        GradientStop {
            id: listenedGS
            position: Math.max(0.0, root.progress)
            color: Configuration.theme.darkGrayColor
        }
        GradientStop {
            id: listenedBoundGS
            property double width: root.calculateBoundWidth() / 2
            position: Math.min(1.0, Math.max(0.0, root.progress + width))
            color: Configuration.theme.grayColor
        }
        GradientStop {
            id: notListenedBoundGS
            property double width: root.calculateBoundWidth() / 2
            property double calculatedPosition: root.progress + listenedBoundGS.width + width

            position: Math.min(1.0, Math.max(0.0, calculatedPosition))
            color: Configuration.theme.lightGrayColor
        }
        GradientStop {
            id: notListenedGS
            position: 1.0
            color: Configuration.theme.lightGrayColor
        }
    }

    HoverHandler {
        id: hoverHandler
    }

    TapHandler {
        onTapped: eventPoint => {
            SomeMediaPlayer.position = //
            SomeMediaPlayer.getPositionFromRatio(root.getProgressRatio(eventPoint.position.x));
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
        if (root.width <= 0) {
            return 0.000;
        }
        let ratio = position / root.width;

        return Math.max(0.000, Math.min(1.000, ratio));
    }
}
