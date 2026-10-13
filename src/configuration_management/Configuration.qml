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
    readonly property int h1PointSize: scale(h1PointSizeStatic)
    readonly property int h2PointSize: scale(h1PointSizeStatic)
    readonly property int h3PointSize: scale(h3PointSizeStatic)
    readonly property int h4PointSize: scale(h4PointSizeStatic)
    readonly property int h5PointSize: scale(h5PointSizeStatic)
    readonly property int h1PointSizeStatic: 50
    readonly property int h2PointSizeStatic: 45
    readonly property int h3PointSizeStatic: 40
    readonly property int h4PointSizeStatic: 35
    readonly property int h5PointSizeStatic: 30

    // стандартные радиусы
    readonly property int r1Radius: scale(r1RadiusStatic)
    readonly property int r2Radius: scale(r2RadiusStatic)
    readonly property int r3Radius: scale(r3RadiusStatic)
    readonly property int r4Radius: scale(r4RadiusStatic)
    readonly property int r5Radius: scale(r5RadiusStatic)

    readonly property int r1RadiusStatic: 40
    readonly property int r2RadiusStatic: 30
    readonly property int r3RadiusStatic: 20
    readonly property int r4RadiusStatic: 15
    readonly property int r5RadiusStatic: 10

    readonly property int primaryTextPointSize: scale(primaryTextPointSizeStatic) // осбоважный укрупнённый текст
    readonly property int regularTextPointSize: scale(regularTextPointSizeStatic) // базовый текст
    readonly property int secondaryTextPointSize: scale(secondaryTextPointSizeStatic) // второстепенный текст
    readonly property int primaryTextPointSizeStatic: 30 // осбоважный укрупнённый текст
    readonly property int regularTextPointSizeStatic: 20 // базовый текст
    readonly property int secondaryTextPointSizeStatic: 15 // второстепенный текст

    readonly property string baseFontFamily: "Times New Roman"
}
