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

    signal accepted
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

        TextField {
            id: textField
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: Configuration.theme.blackColor
            placeholderTextColor: Configuration.theme.grayColor
            font.pointSize: Configuration.regularTextPointSize
            leftPadding: backgrountRect.radius
            rightPadding: leftPadding
            onAccepted: root.accepted()
            onEditingFinished: root.editingFinished()
            onTextEdited: root.textEdited()

            background: Rectangle {
                id: backgrountRect
                color: Configuration.theme.mutedWhiteColor
                border.width: Configuration.scale(2)
                border.color: root.correct ? Configuration.theme.grayColor : Configuration.theme.redColor
                radius: Configuration.r4Radius
            }

            // плейсходлер для полей с масками
            Label {
                text: textField.placeholderText
                font.pointSize: parent.font.pointSize
                color: Configuration.theme.grayColor
                anchors.left: parent.left
                anchors.leftMargin: parent.leftPadding
                anchors.verticalCenter: parent.verticalCenter

                // Скрываем, если поле в фокусе ИЛИ если пользователь уже ввел хоть один символ
                visible: textField.inputMask !== "" && !textField.activeFocus && textField.acceptableInput === false
                background: Rectangle {
                    color: backgrountRect.color
                }
            }
        }
    }
}
