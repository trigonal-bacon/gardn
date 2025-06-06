#pragma once

#include <cstdint>

namespace Ui {
    class Element;
    namespace Panel {
        enum {
            kNone,
            kSettings,
            kPetals,
            kMobs,
            kChangelog
        };
        extern Element *settings;
        extern Element *petal_gallery;
        extern Element *mob_gallery;
        extern Element *changelog;
    }
    extern double dt;
    extern double window_width;
    extern double window_height;
    extern double scale;
    extern double lerp_amount;
    extern Element *focused;
    extern Element *pressed;
    extern uint8_t panel_open;
};