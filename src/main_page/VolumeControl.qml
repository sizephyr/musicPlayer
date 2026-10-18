import QtQuick
import modules.common
import modules.configuration

BaseSlider {
    id: root

    orientation: BaseSlider.Orientation.BottomToTop

    value: Configuration.volumeRatio

    Connections {
        target: root
        function onValueChanged() {
            if (Configuration.volumeRatio !== root.value) {
                Configuration.volumeRatio = root.value;
            }
        }
    }
}
