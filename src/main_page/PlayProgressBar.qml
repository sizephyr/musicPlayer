import QtQuick
import QtQuick.Layouts
import modules.configuration
import modules.main_page

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent
        spacing: Configuration.scale(5)

        PlayProgressBarTimersLine {
            Layout.fillWidth: true
            Layout.preferredHeight: 10
            hovered: playProgressBar.hovered
        }

        PlayProgressBarTimeline {
            id: playProgressBar

            Layout.fillWidth: true
            Layout.preferredHeight: 10
        }
    }
}
