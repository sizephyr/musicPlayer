pragma Singleton

import QtQuick
import QtMultimedia

MediaPlayer {
    id: mediaPlayer

    property double progressRatio: getProgressRatio(mediaPlayer.position)

    audioOutput: AudioOutput {
        // TODO: привязать значение из ProgressBar
        // volume: volumeSlider.value
        volume: 0.1
    }

    onErrorOccurred: (error, errorString) => {
        console.log("Ошибка воспроизведения: " + errorString);
    }

    function getProgressRatio(position) {
        if (mediaPlayer.duration <= 0) {
            return 0.000;
        }
        let ratio = position / mediaPlayer.duration;

        // Ограничиваем значение в пределах от 0 до 1 на случай микросбоев тайминга
        return Math.max(0.000, Math.min(1.000, ratio));
    }
}
