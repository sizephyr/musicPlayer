import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    property bool hovered

    function formatTime(ms) {
        if (ms <= 0 || isNaN(ms))
            return "00:00";
        let totalSeconds = Math.floor(ms / 1000);
        let minutes = Math.floor(totalSeconds / 60);
        let seconds = totalSeconds % 60;

        // Добавляем ведущие нули
        let minStr = minutes < 10 ? "0" + minutes : minutes;
        let secStr = seconds < 10 ? "0" + seconds : seconds;

        return minStr + ":" + secStr;
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Configuration.scale(10)
        anchors.rightMargin: Configuration.scale(10)

        Label {
            id: currentPlayTime
            text: root.formatTime(SomeMediaPlayer.position)
            color: Configuration.theme.lightGrayColor
            visible: root.hovered
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Label {
            id: fullTrackTime
            text: root.formatTime(SomeMediaPlayer.duration)
            color: Configuration.theme.lightGrayColor
            visible: root.hovered
        }
    }
}
