import QtQuick
import modules.common
import modules.main_page

BaseSlider {
    id: root

    orientation: BaseSlider.Orientation.BottomToTop

    value: SomeMediaPlayer.audioOutput.volume

    Connections {
        target: root
        function onValueChanged() {
            if (SomeMediaPlayer.audioOutput.volume !== root.value) {
                SomeMediaPlayer.audioOutput.volume = root.value;
            }
        }
    }
}
