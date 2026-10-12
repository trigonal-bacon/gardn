#include <Client/Ui/Extern.hh>

#include <Client/Ui/Element.hh>

namespace Ui {
    namespace Panel {
        Element::Handle<Element> settings = nullptr;
        Element::Handle<Element> petal_gallery = nullptr;
        Element::Handle<Element> mob_gallery = nullptr;
        Element::Handle<Element> changelog = nullptr;
    }
    double dt = 0;
    double window_width = 1920;
    double window_height = 1080;
    double scale = 1;
    double lerp_amount = 0.05;
    Element::Handle<Element> focused = nullptr;
    Element::Handle<Element> pressed = nullptr;
    uint8_t panel_open = Panel::kNone;
}