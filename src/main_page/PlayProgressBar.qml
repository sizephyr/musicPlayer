import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import modules.common
import modules.configuration
import modules.page_manager

Rectangle {
    id: playProgressBar

    Layout.preferredHeight: 10

    border.width: 2
    border.color: Configuration.theme.semiTransparentMutedWhiteColor
    radius: Configuration.r3Radius

    readonly property double progressBegin: 0.0
    property double progress: SomeMediaPlayer.progressRatio

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

    gradient: Gradient {
        orientation: Gradient.Horizontal

        GradientStop {
            position: playProgressBar.progressBegin
            color: Configuration.theme.darkGrayColor
        }
        GradientStop {
            id: listenedGS
            position: Math.max(0.0, playProgressBar.progress)
            color: Configuration.theme.darkGrayColor
        }
        GradientStop {
            id: listenedBoundGS
            property double width: playProgressBar.calculateBoundWidth() / 2
            position: Math.min(1.0, Math.max(0.0, playProgressBar.progress + width))
            color: Configuration.theme.grayColor
        }
        GradientStop {
            id: notListenedBoundGS
            property double width: playProgressBar.calculateBoundWidth() / 2
            property double calculatedPosition: playProgressBar.progress + listenedBoundGS.width + width

            position: Math.min(1.0, Math.max(0.0, calculatedPosition))
            color: Configuration.theme.lightGrayColor
        }
        GradientStop {
            id: notListenedGS
            position: 1.0
            color: Configuration.theme.lightGrayColor
        }
    }
}
