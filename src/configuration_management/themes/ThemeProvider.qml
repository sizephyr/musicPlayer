import QtQuick 2.2

QtObject {
    // --- Абсолютные контрасты (Modern UI) ---
    readonly property color whiteColor: "#ffffff"
    readonly property color blackColor: "#000000"

    // --- Оттенки серого (Интерфейсные) ---
    readonly property color darkGrayColor: "#39414a"
    readonly property color grayColor: "#7e8798"
    readonly property color lightGrayColor: "#eceef4"
    readonly property color mutedWhiteColor: "#fbfbfd"

    // --- Красная группа ---
    readonly property color darkRedColor: "#c94648"
    readonly property color redColor: "#fe6f71"
    readonly property color lightRedColor: "#f6e4e4"

    // --- Зеленая группа ---
    readonly property color darkGreenColor: "#256a71"
    readonly property color greenColor: "#2f8e97"
    readonly property color lightGreenColor: "#e8f2f2"

    // --- Желтая группа ---
    readonly property color darkYellowColor: "#b38600"
    readonly property color yellowColor: "#fecc00"
    readonly property color lightYellowColor: "#fffbe6"

    readonly property color semiTransparentWhiteColor: "#aaffffff"
    readonly property color semiTransparentGrayColor: "#aa7e8798"
    readonly property color semiTransparentDarkGrayColor: "#aa39414a"
    readonly property color semiTransparentMutedWhiteColor: "#aafbfbfd"
    readonly property color semiTransparentLightGrayColor: "#77eceef4"

    property double backgroundTransparentnesRatio: 0.5

    function extrapolateAlpha(inputColor, transparencyFactor) {
        var factor = Math.max(0.0, Math.min(1.0, transparencyFactor));

        var targetAlpha = 1.0 - factor;

        return Qt.rgba(inputColor.r, inputColor.g, inputColor.b, targetAlpha);
    }
}
