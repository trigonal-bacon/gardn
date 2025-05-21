#include <Client/Ui/Element.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Input.hh>

#include <cmath>

using namespace Ui;

Element::Element(float w, float h, Style s) : width(w), height(h), style(s) {
    //so the animation doesnt set
    animation.set(1);
    animate = [&](Renderer &ctx){ ctx.scale((float) animation); };
}

void Element::render(Renderer &ctx) {
    //get abs x, y;
    screen_x = ctx.context.transform_matrix[2];
    screen_y = ctx.context.transform_matrix[5];
    animation.set(should_render());
    if (showed) animation.step(1 - pow(1 - 0.3, dt * 60 / 1000));
    else animation.step(1);
    float curr_animation = (float) animation;
    visible = curr_animation > 0.001;
    if (visible) {
        RenderContext context(&ctx);
        ctx.set_stroke(0x80000000);
        ctx.set_line_width(1);
        ctx.begin_path();
        ctx.round_rect(-width / 2, -height / 2, width, height, 6);
        ctx.stroke();
    }
    if (visible) {
        animate(ctx);
        on_render(ctx);
        showed = 1;
        //possibly poll events?
    } else 
        on_render_skip(ctx);
    //event emitter
    if (Ui::focused == this) {
        if (BIT_AT(Input::mouse_buttons_pressed, Input::LeftMouse)) {
            focus_state = kMouseDown;
            on_event(kMouseDown);
        }
        else if (BIT_AT(Input::mouse_buttons_released, Input::LeftMouse)) {
            focus_state = kClick;
            on_event(kClick);
        }
        else {
            focus_state = kMouseHover;
            on_event(kMouseHover);
        }
    } else if (focus_state != kFocusLost) {
        focus_state = kFocusLost;
        on_event(kFocusLost);
    }
    //focus_state = kFocusLost;
}

void Element::on_render(Renderer &ctx) {
    if (style.fill != 0x00000000) {
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
}

void Element::on_render_skip(Renderer &ctx) {}

void Element::on_event(uint8_t event) {}

void Element::refactor() {}

void Element::poll_events() {
    if (fabsf(Input::mouse_x - screen_x) < width * Ui::scale / 2
    && fabsf(Input::mouse_y - screen_y) < height * Ui::scale / 2) {
        Ui::focused = this;
    }
    else if (Ui::focused == this) {
        Ui::focused = nullptr;
    }
}