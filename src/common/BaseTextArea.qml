import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import modules.configuration

Item {
    id: root

    property string fieldName
    property bool requiredField: false
    property alias textInput: textField
    property bool correct: true

    signal editingFinished
    signal textEdited

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: root.requiredField ? root.fieldName + " *" : root.fieldName
            color: Configuration.theme.grayColor
            font.pointSize: Configuration.secondaryTextPointSize
            font.bold: true
            leftPadding: backgrountRect.radius
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            // todo обдумать поведение (скорее всего, вписать с учётом скругления
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff

            TextArea {
                id: textField
                color: Configuration.theme.blackColor
                placeholderTextColor: Configuration.theme.grayColor
                font.pointSize: Configuration.regularTextPointSize
                leftPadding: backgrountRect.radius
                rightPadding: leftPadding
                onEditingFinished: root.editingFinished()
                onTextChanged: root.textEdited()

                background: Rectangle {
                    id: backgrountRect
                    color: Configuration.theme.mutedWhiteColor
                    border.width: 2
                    border.color: root.correct ? Configuration.theme.lightGrayColor : Configuration.theme.redColor
                    radius: Configuration.r4Radius
                }
            }
        }
    }
}
