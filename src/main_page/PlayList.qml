import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Qt5Compat.GraphicalEffects
import modules.common
import modules.configuration

Item {
    id: root

    property var model

    signal selected(var trackUid)

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

            delegate: PlayListItem {
                width: view.width
                height: (view.height - view.anchors.margins * 2) / 7
                leftPadding: view.anchors.margins
                text: model.trackArtist + " - " + model.trackTitle

                onSelectedChanged: {
                    if (selected)
                        root.selected(model.uid);
                }
                Connections {
                    target: root

                    function onSelected(uid) {
                        selected = (model.uid === uid);
                    }
                }
            }
        }
    }
}
