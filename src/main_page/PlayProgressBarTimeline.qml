import QtQuick
import modules.common
import modules.main_page

BaseSlider {
    id: root

    property HoverHandler hoverHandler: HoverHandler {}
    property alias hovered: root.hoverHandler.hovered

    Binding {
        target: root
        property: "value"
        value: SomeMediaPlayer.progressRatio
        when: !root.trackHandler.active
    }

    orientation: BaseSlider.Orientation.LeftToRight

    Connections {
        target: root
        function onValueChanged() {
            if (SomeMediaPlayer.progressRatio !== root.value) {
                SomeMediaPlayer.position = SomeMediaPlayer.getPositionFromRatio(root.value);
            }
        }
    }
}
