import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.configuration
import modules.common

Item {
    id: root

    // высота условной "единицы"
    property int baseHeight
    property int buttonsCount: 3
    // высота контента из расчёта кол-ва кнопок + 1 высота для того, чтобы всё выглядывало из
    // под заголовка
    property int contentHeight: (baseHeight * buttonsCount) + baseHeight
    property bool expanded: false
    property bool devicesOk: true
    property bool connectionOk: true
    property color mainColor

    signal showServerState
    signal showDevicesState
    signal showSettings

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: contentRect
            Layout.preferredHeight: root.expanded ? root.contentHeight : 0
            Layout.fillWidth: true
            color: Configuration.theme.whiteColor
            bottomLeftRadius: Configuration.r4Radius
            bottomRightRadius: Configuration.r4Radius
            border.color: root.mainColor
            border.width: 2
            clip: true

            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
            ColumnLayout {
                id: contentLayout
                anchors.fill: parent
                anchors.margins: 10
                anchors.topMargin: baseHeight + anchors.margins

                BaseButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 1

                    icon.source: "pics/settings.png"
                    text: "Настройки"
                    style: BaseButtonStyle.Alternative
                    font.pointSize: Configuration.primaryTextPointSize
                    onClicked: {
                        root.expanded = false;
                        root.showSettings();
                    }
                }
                BaseButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 1

                    icon.color: root.devicesOk ? Configuration.theme.greenColor : Configuration.theme.redColor
                    icon.source: root.devicesOk ? "pics/correct_devices.png" : "pics/wrong_devices.png"
                    text: "Состояние уст-в"
                    font.pointSize: Configuration.primaryTextPointSize

                    onClicked: {
                        root.expanded = false;
                        root.showDevicesState();
                        // ↓↓УБРАТЬ ПОСЛЕ ДЕБАГА↓↓↓↓↓↓↓↓
                        root.devicesOk = !root.devicesOk;
                        // ↑↑УБРАТЬ ПОСЛЕ ДЕБАГА↑↑↑↑↑↑↑↑
                    }
                    style: BaseButtonStyle.Alternative
                }
                BaseButton {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 1

                    icon.color: root.connectionOk ? Configuration.theme.greenColor : Configuration.theme.redColor
                    icon.source: "pics/ellipse.png"
                    text: "Состояние подключения"

                    onClicked: {
                        root.expanded = false;
                        root.showServerState();
                        // ↓↓↓↓↓↓↓↓УБРАТЬ ПОСЛЕ ДЕБАГА↓↓↓↓↓↓↓↓
                        root.connectionOk = !root.connectionOk;
                        // ↑↓↓↓↓↓↓↑УБРАТЬ ПОСЛЕ ДЕБАГА↑↑↑↑↑↑↑↑
                    }
                    style: BaseButtonStyle.Alternative
                }
            }
        }

        Item {
            Layout.preferredHeight: root.expanded ? 0 : root.contentHeight
            Behavior on Layout.preferredHeight {
                NumberAnimation {
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
