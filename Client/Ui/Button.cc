#include <Client/Ui/Button.hh>

using namespace Ui;

Button::Button(float w, float h, Element *l, void x(uint8_t), Style s) :
    Element(w, h, s), label(l), on_click(x)
{}

void Button::on_render(Renderer &ctx) {
    if (style.fill != 0x00000000) {
        if (focus_state == kMouseHover)
            ctx.set_fill(Renderer::HSV(style.fill, 1.1));
        else
            ctx.set_fill(style.fill);
        ctx.set_stroke(Renderer::HSV(style.fill, style.stroke_hsv));
        ctx.set_line_width(style.line_width);
        ctx.round_line_cap();
        ctx.round_line_join();
        ctx.begin_path();
        ctx.round_rect(-width / 2, -height / 2, width, height, style.round_radius);
        ctx.fill();
        if (style.fill >= 0xff000000) ctx.stroke();
    }
    if (label != nullptr) label->render(ctx);
}

void Button::refactor() {
    if (label != nullptr) label->refactor();
}

void Button::on_event(uint8_t event) {
    on_click(event);
}