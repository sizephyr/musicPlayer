import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls.Basic
import modules.notify_manager

ListView {
    id: root
    clip: true
    spacing: 10

    model: NotifyManagerModel.viewModel

    interactive: false

    displaced: Transition {
        NumberAnimation {
            properties: "y"
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }

    delegate: Notify {
        width: root.width - rightMargin
        autoHide: model.autoHide
        title: model.title
        message: model.message
        type: model.type

        onVisibleChanged: {
            if (!visible)
                root.model.remove(index)
        }
    }
}
