import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.impl
import QtQuick.Controls.Basic
import modules.common
import modules.configuration

TextField {
    id: root

    signal filterChanged(string newFilter)

    placeholderText: "Введите текст для поиска"
    leftPadding: searchSymbol.width + searchSymbol.anchors.leftMargin + 10
    rightPadding: leftPadding
    font.pointSize: Configuration.regularTextPointSize
    onTextChanged: filterChanged(text)

    background: Rectangle {
        id: backgroundRect
        color: Configuration.theme.whiteColor
        border.color: Configuration.theme.lightGrayColor
        border.width: 2
        radius: height / 2
    }

    IconImage {
        id: searchSymbol
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: backgroundRect.radius / 2
        width: 30
        color: Configuration.theme.grayColor
        source: "qrc:/qt/qml/modules/common/img/search_bar_magnifier_pic.png"
    }

    Button {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: backgroundRect.radius / 2
        width: 30
        text: "×"
        font.bold: true
        font.pointSize: 24
        palette.buttonText: Configuration.theme.redColor
        background: Rectangle {
            color: "transparent"
        }
        visible: root.text !== ""

        onClicked: root.text = ""
    }
}
