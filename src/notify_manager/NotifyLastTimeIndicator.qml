import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.common
import modules.notify_manager

Rectangle {
    id: timerIndicator
    
    width: parent ? parent.width * progress : 0

    property int visibleInterval

    property real progress: 1.0
    property var startTime: Date.now()
    
    Timer {
        interval: 16
        repeat: true
        running: timerIndicator.visible && timerIndicator.progress > 0
        
        onTriggered: {
            let elapsed = Date.now() - timerIndicator.startTime
            let nextProgress = (root.visibleInterval - elapsed) / root.visibleInterval
            
            if (nextProgress <= 0) {
                timerIndicator.progress = 0
                running = false
            } else {
                timerIndicator.progress = nextProgress
            }
        }
    }
}
