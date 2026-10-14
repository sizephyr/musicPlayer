import QtQuick
import QtQuick.Layouts
import modules.main_page

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        PlayProgressBarTimersLine {
            Layout.fillWidth: true
            Layout.preferredHeight: 15
            hovered: playProgressBar.hovered
        }

        PlayProgressBarTimeline {
            id: playProgressBar

            Layout.fillWidth: true
            Layout.preferredHeight: 15
        }
    }
}
