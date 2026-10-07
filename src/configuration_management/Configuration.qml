pragma Singleton

import QtQuick 2.2
import modules.themes
import modules.configuration

ConfigurationBase {
    id: root

    property ThemeProvider theme: ThemeProvider {}

    function scale(value) {
        return value * scaleFactor;
    }

    // размер заголовка относительно высоты окна
    readonly property int h1PointSize: scale(50)
    readonly property int h2PointSize: scale(45)
    readonly property int h3PointSize: scale(40)
    readonly property int h4PointSize: scale(35)
    readonly property int h5PointSize: scale(30)

    // стандартные радиусы
    readonly property int r1Radius: scale(40)
    readonly property int r2Radius: scale(30)
    readonly property int r3Radius: scale(20)
    readonly property int r4Radius: scale(15)
    readonly property int r5Radius: scale(10)

    readonly property int primaryTextPointSize: scale(30) // осбоважный укрупнённый текст
    readonly property int regularTextPointSize: scale(20) // базовый текст
    readonly property int secondaryTextPointSize: scale(15) // второстепенный текст

    readonly property string baseFontFamily: "Times New Roman"
}
