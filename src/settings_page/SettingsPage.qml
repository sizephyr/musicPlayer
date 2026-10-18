import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration

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

                SettingsSlider {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: "Прозрачность фона"

                    value: Configuration.backgroundTransparentnesRatio
                    onValueChanged: {
                        if (Configuration.backgroundTransparentnesRatio !== value) {
                            Configuration.backgroundTransparentnesRatio = value;
                        }
                    }
                }
                SettingsFileSellector {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: "Фоновая картинка"

                    currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                    defaultSuffix: "png"
                    onAccepted: Configuration.windowBackground = selectedFile
                }
                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
