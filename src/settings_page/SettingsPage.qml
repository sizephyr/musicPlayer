import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration
import modules.common

// NOTE: пока что просто имитируем наличие модель-вью: особо нечего конфигурировать
Popup {
    id: root

    modal: true
    focus: true
    dim: false

    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    background: Rectangle {
        color: Configuration.theme.extrapolateAlpha(Configuration.theme.darkGrayColor, 0.1)
        radius: Configuration.r3RadiusStatic
        border.width: 2
        border.color: Configuration.theme.blackColor
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 10
            Layout.rightMargin: 10

            ColumnLayout {
                anchors.fill: parent
                SectionItem {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: "Основные"
                }
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 2
            color: Configuration.theme.blackColor
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 20
            Layout.leftMargin: 10

            ColumnLayout {
                anchors.fill: parent

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    RowLayout {
                        anchors.fill: parent

                        Label {
                            Layout.preferredHeight: 40
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            background: Rectangle{
                                color:"transparent"
                            }

                            text: "Прозрачность фона"
                        }
                        BaseSlider{
                            id: slider
                            Layout.fillWidth: true
                            Layout.preferredWidth: 40
                            Layout.preferredHeight: 30
                            Layout.margins: 25
                            Layout.leftMargin: 10
                            Layout.rightMargin: 10
                            radius: Configuration.r5RadiusStatic
                            orientation: BaseSlider.Orientation.LeftToRight

                            value: Configuration.theme.backgroundTransparentnesRatio
                            Connections {
                                target: slider
                                function onValueChanged() {
                                    if (Configuration.theme.backgroundTransparentnesRatio !== slider.value) {
                                        Configuration.theme.backgroundTransparentnesRatio = slider.value;
                                    }
                                }
                            }
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
