import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import modules.common
import modules.configuration
import modules.page_manager
import modules.main_page

Item {
    Item {
        anchors.fill: parent
        anchors.margins: Configuration.scale(100)
        anchors.rightMargin: 0
        
        ColumnLayout {
            anchors.fill: parent
            spacing: Configuration.scale(30)
            
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: width
                
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: Configuration.scale(20)
                    
                    border.width: Configuration.scale(6)
                    border.color: Configuration.theme.semiTransparentMutedWhiteColor
                    color: Configuration.theme.semiTransparentLightGrayColor
                    radius: Configuration.r3Radius
                }
            }
            
            PlayProgressBar {
                id: playProgressBar
                Layout.fillWidth: true
            }
        }
    }
}
