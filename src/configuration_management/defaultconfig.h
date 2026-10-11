#pragma once

namespace Configuration::Private {
static constexpr auto defaultConfig = R"({
    "window": {
        "width": 1000,
        "height": 600,
        "title": "player",
        "background": ":/qt/qml/Main/img/base_background.jpg"
    },
    "animaltion": {
        "duration_ms": 250,
        "notification_hide_after_ms": 4000
    },
    "network": {
        "base_transfer_timeout_ms": 1000
    }
})";
}