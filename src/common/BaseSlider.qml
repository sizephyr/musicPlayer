import QtQuick
import modules.configuration

Rectangle {
    id: root

    enum Orientation {
        BottomToTop,
        LeftToRight
    }

    border.width: 1
    border.color: Configuration.theme.semiTransparentLightGrayColor
    radius: Configuration.r3RadiusStatic
    opacity: canShow ? 1.0 : 0.0

    property bool canShow: true
    property double value
    property int orientation: BaseSlider.Orientation.BottomToTop

    property PointHandler trackHandler: PointHandler {
        target: null

        onPointChanged: {
            if (root.trackHandler.active) {
                if (root.orientation === BaseSlider.Orientation.BottomToTop) {
                    let currentY = root.trackHandler.point.position.y;
                    root.value = root.getProgressRatio(root.height - currentY);
                } else {
                    let currentX = root.trackHandler.point.position.x;
                    root.value = root.getProgressRatio(currentX);
                }
            }
        }
    }

    gradient: Gradient {
        orientation: root.orientation === BaseSlider.Orientation.BottomToTop ? Gradient.Vertical : Gradient.Horizontal

        GradientStop {
            position: 0.0
            color: root.orientation === BaseSlider.Orientation.BottomToTop ? Configuration.theme.lightGrayColor : Configuration.theme.darkGrayColor
        }
        GradientStop {
            property double width: root.calculateBoundWidth() / 2
            property double calculatedPosition: root.value + controlBoundGS.width + width

            position: root.orientation === BaseSlider.Orientation.BottomToTop ? Math.max(0.0, Math.min(1.0, 1.0 - calculatedPosition)) : Math.max(0.0, Math.min(1.0, root.value))
            color: root.orientation === BaseSlider.Orientation.BottomToTop ? Configuration.theme.lightGrayColor : Configuration.theme.darkGrayColor
        }
        GradientStop {
            id: controlBoundGS
            property double width: root.calculateBoundWidth() / 2

            position: root.orientation === BaseSlider.Orientation.BottomToTop ? Math.max(0.0, Math.min(1.0, 1.0 - (root.value + width))) : Math.max(0.0, Math.min(1.0, root.value + width))
            color: Configuration.theme.grayColor
        }
        GradientStop {
            position: root.orientation === BaseSlider.Orientation.BottomToTop ? Math.max(0.0, Math.min(1.0, 1.0 - root.value)) : Math.max(0.0, Math.min(1.0, root.value + controlBoundGS.width + (root.calculateBoundWidth() / 2)))
            color: root.orientation === BaseSlider.Orientation.BottomToTop ? Configuration.theme.darkGrayColor : Configuration.theme.lightGrayColor
        }
        GradientStop {
            position: 1.0
            color: root.orientation === BaseSlider.Orientation.BottomToTop ? Configuration.theme.darkGrayColor : Configuration.theme.lightGrayColor
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: Configuration.animationDurationMs()
            easing.type: Easing.InOutQuad
        }
    }

    function calculateBoundWidth() {
        const p = Math.max(0.0, Math.min(1.0, value));

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
        let totalLength = root.orientation === BaseSlider.Orientation.BottomToTop ? root.height : root.width;

        if (totalLength <= 0) {
            return 0.000;
        }
        let ratio = position / totalLength;

        return Math.max(0.000, Math.min(1.000, ratio));
    }
}
