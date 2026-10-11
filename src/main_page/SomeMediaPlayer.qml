pragma Singleton

import QtQuick
import QtMultimedia

MediaPlayer {
    id: root

    property double progressRatio: getProgressRatio(position)

    audioOutput: AudioOutput {
        // TODO: привязать значение громкости из ProgressBar
        // volume: volumeSlider.value
        volume: 0.1
    }

    onErrorOccurred: (error, errorString) => {
        console.log("Ошибка воспроизведения: " + errorString);
    }

    function getProgressRatio(position) {
        if (root.duration <= 0) {
            return 0.000;
        }
        let ratio = position / root.duration;

        return Math.max(0.000, Math.min(1.000, ratio));
    }
    function getPositionFromRatio(ratio) {
        if (root.duration <= 0) {
            return 0;
        }

        let clampedRatio = Math.max(0.0, Math.min(1.0, ratio));

        return Math.round(clampedRatio * root.duration);
    }
}
