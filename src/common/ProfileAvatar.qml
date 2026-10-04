import QtQuick 2.15
import Qt5Compat.GraphicalEffects
import modules.common.lib
import modules.configuration

Image {
    id: root

    property int sex: Sex.Undefined

    readonly property color maleColor: Configuration.theme.greenColor
    readonly property color femaleColor: Configuration.theme.redColor
    readonly property string maleIconSource: "qrc:/qt/qml/modules/common/img/male_pic.png"
    readonly property string femaleIconSource: "qrc:/qt/qml/modules/common/img/female_pic.png"
    readonly property string unknownIconSource: "qrc:/qt/qml/modules/common/img/unknown_sex_icon_pic.png"

    source: {
        if (root.sex === Sex.Male)
            return maleIconSource
        else if (root.sex === Sex.Female)
            return femaleIconSource
        else
            return unknownIconSource
    }
    width: height
    fillMode: Image.PreserveAspectFit

    layer.enabled: true
    layer.effect: ColorOverlay {
        color: {
            if (root.sex === Sex.Male)
                return maleColor
            else if (root.sex === Sex.Female)
                return femaleColor
            else
                return Configuration.theme.grayColor
        }
    }
}
