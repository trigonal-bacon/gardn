#include <Client/Ui/Button.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Input.hh>

using namespace Ui;

Button::Button(float w, float h, Element *l, void x(Element *, uint8_t), bool y (void), Style s) :
    Element(w, h, s), on_click(x), should_darken(y)
{
    if (l != nullptr) children.push_back(l);
}

void Button::on_render(Renderer &ctx) {
    if (style.fill != 0x00000000) {
        if (should_darken != nullptr && should_darken())
            ctx.set_fill(Renderer::HSV(style.fill, 0.9));
        else if (focus_state == kMouseHover)
            ctx.set_fill(Renderer::HSV(style.fill, 1.1));
        else if (focus_state == kMouseDown)
            ctx.set_fill(Renderer::HSV(style.fill, 0.9));
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
    for (Element *child : children) child->render(ctx);
}

void Button::refactor() {
    for (Element *child : children) child->refactor();
}

void Button::on_event(uint8_t event) {
    on_click(this, event);
}

ToggleButton::ToggleButton(float w, uint8_t *t) : 
    Element(w,w,{ .fill = 0xff666666, .stroke_hsv = 0.4, .line_width = 4, .round_radius = 5 }), toggler(t) {
    lerp_toggle = 0;
}

void ToggleButton::on_render(Renderer &ctx) {
    lerp_toggle = lerp(lerp_toggle, *toggler, Ui::lerp_amount * 2);
    ctx.set_fill(Renderer::HSV(style.fill, style.stroke_hsv));
    ctx.begin_path();
    ctx.round_rect(-width / 2, -height / 2, width, height, style.round_radius);
    ctx.fill();
    ctx.set_fill(Renderer::MIX(style.fill, 0xffcfcfcf, lerp_toggle));
    ctx.begin_path();
    ctx.rect(-width / 2 + style.line_width, -height / 2 + style.line_width, width - 2 * style.line_width, height - 2 * style.line_width);
    ctx.fill();
}


void ToggleButton::on_event(uint8_t event) {
    if (event == kClick) *toggler ^= 1;
}