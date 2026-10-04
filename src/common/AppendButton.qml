import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.impl
import QtQuick.Controls.Basic
import modules.common

BaseButton {
    id: root

    IconImage {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: 15
        width: height / 2.7
        color: palette.buttonText
        source: "qrc:/qt/qml/modules/common/img/append_pic.png"
    }
}
