#include <Client/Ui/Extern.hh>

#include <Client/Ui/Element.hh>

namespace Ui {
    double timestamp = 0;
    double dt = 0;
    double window_width = 1920;
    double window_height = 1080;
    double scale = 1;
    double lerp_amount = 0.05;
    Element *focused = nullptr;
    Element *pressed = nullptr;
}