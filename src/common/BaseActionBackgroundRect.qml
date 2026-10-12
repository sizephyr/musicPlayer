import QtQuick
import modules.configuration

Rectangle {
    id: root

    color: Configuration.theme.semiTransparentGrayColor
    radius: Configuration.r5Radius

    // на момент Component.onCompleted размеры ещё просто не успели примениться
    // onHeightChanged: {
    //     if (height > 0 && !appearanceAnimation.running && translateTransform.y === 0) {
    //         show();
    //     }
    // }

    readonly property int baseRadius: Configuration.r1Radius

    property bool onCloseProcess: false
    // функция, которая будет единоразово вызвана после проигрывания анимации скрытия
    property var callAfterHide

    signal animationFinished
    signal closed

    transform: Translate {
        id: translateTransform
    }
    NumberAnimation {
        id: appearanceAnimation
        target: translateTransform
        property: "y"
        duration: Configuration.animationDurationMs()
        easing.type: Easing.OutCubic

        onStarted: {
            if (appearanceAnimation.to === 0)
                visible = true;
        }

        onFinished: {
            if (translateTransform.to === root.height)
                visible = false;
            root.animationFinished();
            if (onCloseProcess)
                root.closed();
            if (root.callAfterHide) {
                root.callAfterHide();
                root.callAfterHide = undefined;
            }
        }
    }

    function show() {
        if (appearanceAnimation.running)
            appearanceAnimation.stop();

        appearanceAnimation.from = root.height;
        appearanceAnimation.to = 0;
        appearanceAnimation.restart();
    }

    function hide() {
        if (appearanceAnimation.running)
            appearanceAnimation.stop();

        appearanceAnimation.from = 0;
        appearanceAnimation.to = root.height;
        appearanceAnimation.restart();
    }

    function hideAndCall(callAfterHide) {
        root.callAfterHide = callAfterHide;
        hide();
    }

    function close() {
        onCloseProcess = true;
        hide();
    }
}
