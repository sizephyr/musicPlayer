import QtQuick
import modules.configuration
import modules.main_page

Item {
    id: root

    // TODO: надо бы навести порядок. У плейлиста должна быть только модель плейлиста
    property MainPageModel model
    property Item contentItem: contentRect
    property int count: view.count
    default property alias content: contentRect.data

    signal appendFileCalled
    signal appendFolderCalled

    Rectangle {
        anchors.centerIn: parent
        width: 500
        height: 500
        color: "transparent"

        HoverHandler {
            id: hoverHandler
        }

        Rectangle {
            id: contentRect
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.75
            height: parent.height * 0.9

            border.width: 1
            border.color: Configuration.theme.extrapolateAlpha(Configuration.theme.mutedWhiteColor, 0.3)
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
                    editState: view.model.editState

                    onSelect: {
                        root.model.play(uid);
                    }
                    onRemove: {
                        root.model.remove(uid);
                    }
                    onSelectedChanged: {
                        if (selected)
                            view.currentIndex = index;
                    }
                }
            }
            OpenCurrentFolderBtn {
                id: openCurrentFolderBtn
                anchors.fill: parent
                onClicked: root.appendFolderCalled()
                visible: view.count === 0
            }
        }

        PlayListActions {
            id: actions
            anchors.left: contentRect.right
            anchors.verticalCenter: parent.verticalCenter
            height: contentRect.height - contentRect.radius * 2
            width: hoverHandler.hovered ? parent.width * 0.10 : 0
            playlistEmpty: view.count === 0

            onAppendFileCalled: root.appendFileCalled()
            onAppendFolderCalled: root.appendFolderCalled()
            onEditCalled: root.model.playListModel.editState = !root.model.playListModel.editState
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
            function onStopped() {
                SomeMediaPlayer.source = "";
                SomeMediaPlayer.stop();
            }
        }
    }
}
