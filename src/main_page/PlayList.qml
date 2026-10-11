import QtQuick
import modules.configuration
import modules.main_page

Item {
    id: root

    // TODO: надо бы навести порядок. У плейлиста должна быть только модель плейлиста
    property MainPageModel model
    default property alias content: contentRect.data

    Rectangle {
        id: contentRect
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

            model: root.model.playListModel

            delegate: PlayListItem {
                width: view.width
                height: (view.height - view.anchors.margins * 2) / 7
                leftPadding: view.anchors.margins
                text: model.trackArtist + " - " + model.trackTitle
                selected: model.uid === root.model.playListModel.currentTrackUid

                onSelect: {
                    root.model.play(uid);
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
                // TODO
                // playContext.coverSource = "";
                SomeMediaPlayer.stop();
            }
        }
    }
}
