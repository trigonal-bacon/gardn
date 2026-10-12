#pragma once

#include <Client/Ui/Element.hh>

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
        extern Element::Handle<Element> settings;
        extern Element::Handle<Element> petal_gallery;
        extern Element::Handle<Element> mob_gallery;
        extern Element::Handle<Element> changelog;
    }
    extern double dt;
    extern double window_width;
    extern double window_height;
    extern double scale;
    extern double lerp_amount;
    extern Element::Handle<Element> focused;
    extern Element::Handle<Element> pressed;
    extern uint8_t panel_open;
};