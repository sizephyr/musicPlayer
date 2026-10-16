import QtQuick
import modules.configuration
import modules.main_page

Item {
    id: root

    // TODO: надо бы навести порядок. У плейлиста должна быть только модель плейлиста
    property MainPageModel model
    property Item contentItem: contentRect
    default property alias content: contentRect.data

    Rectangle {
        anchors.centerIn: parent
        width: 500
        height: 500

        color: "transparent"

        Rectangle {
            id: contentRect
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.75
            height: parent.height * 0.9

            border.width: 1
            border.color: Configuration.theme.semiTransparentLightGrayColor
            color: Configuration.theme.semiTransparentLightGrayColor
            radius: Configuration.r3RadiusStatic

            ListView {
                id: view

                anchors.fill: parent
                anchors.margins: parent.radius
                clip: true
                spacing: 6

                highlightRangeMode: ListView.ApplyRange
                highlightMoveDuration: 300

                model: root.model.playListModel

                delegate: PlayListItem {
                    width: view.width
                    height: (view.height - view.anchors.margins * 2) / 7
                    horizontalPaddings: view.anchors.margins
                    text: model.trackArtist + " - " + model.trackTitle
                    selected: model.uid === root.model.playListModel.currentTrackUid

                    onSelect: {
                        root.model.play(uid);
                    }
                    onSelectedChanged: {
                        if (selected)
                            view.currentIndex = index;
                    }
                }
            }
        }

        Connections {
            target: root.model

            function onPlayed(uid) {
                let trackPath = root?.model?.trackPathOf(uid) ?? "";

                if (trackPath !== "") {
                    SomeMediaPlayer.source = "file://" + trackPath;
                    SomeMediaPlayer.play();
                } else {
                    SomeMediaPlayer.stop();
                }
            }
        }
    }
}
