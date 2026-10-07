import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import modules.common
import modules.configuration

Item {
    id: root

    property var model

    Rectangle {
        anchors.fill: parent
        anchors.margins: Configuration.scale(100)
        anchors.leftMargin: 0

        border.width: Configuration.scale(6)
        border.color: Configuration.theme.semiTransparentMutedWhiteColor
        color: Configuration.theme.semiTransparentLightGrayColor
        radius: Configuration.r3Radius

        ListView {
            id: view

            anchors.fill: parent
            anchors.margins: parent.radius
            clip: true
            spacing: Configuration.scale(6)

            model: root.model

            delegate: Label {
                width: view.width
                height: (view.height - view.anchors.margins * 2) / 7
                verticalAlignment: Text.AlignVCenter
                leftPadding: view.anchors.margins

                text: model.trackArtist + " - " + model.trackTitle
                font.pointSize: Configuration.regularTextPointSize
                color: Configuration.theme.darkGrayColor

                background: Rectangle {
                    border.width: 1
                    border.color: Configuration.theme.semiTransparentLightGrayColor
                    color: "transparent"
                    radius: Configuration.r5Radius
                }

                Button {
                    anchors.fill: parent
                    background: Rectangle {
                        color: "transparent"
                        radius: Configuration.r5Radius
                    }
                }
            }
        }
    }
}
