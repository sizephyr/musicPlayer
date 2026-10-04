import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.common
import modules.common.lib
import modules.patients_page
import modules.configuration

Label {
    id: root

    property int statusColor: StatusLabelColor.Gray
    property string statusText

    font.family: Configuration.baseFontFamily
    font.pointSize: Configuration.secondaryTextPointSize
    verticalAlignment: Qt.AlignVCenter
    font.bold: true
    color: {
        if (root.statusColor === StatusLabelColor.Gray)
            Configuration.theme.grayColor;
        else if (root.statusColor === StatusLabelColor.LightGreen)
            Configuration.theme.darkGreenColor;
        else if (root.statusColor === StatusLabelColor.LightRed)
            Configuration.theme.darkRedColor;
        else if (root.statusColor === StatusLabelColor.LightYellow)
            Configuration.theme.darkYellowColor;
    }
    text: root.statusText
    leftPadding: 10
    rightPadding: 10
    topPadding: 5
    bottomPadding: 5

    background: Rectangle {
        radius: width / 2
        color: {
            if (root.statusColor === StatusLabelColor.Gray)
                Configuration.theme.lightGrayColor;
            else if (root.statusColor === StatusLabelColor.LightGreen)
                Configuration.theme.lightGreenColor;
            else if (root.statusColor === StatusLabelColor.LightRed)
                Configuration.theme.lightRedColor;
            else if (root.statusColor === StatusLabelColor.LightYellow)
                Configuration.theme.lightYellowColor;
        }
    }
}
