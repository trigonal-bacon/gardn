#include <Client/Ui/Element.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Input.hh>

#include <cmath>

using namespace Ui;

Element::Element(float w, float h, Style s) : width(w), height(h), style(s) {
    animation = 1;
}

void Element::animate(Renderer &ctx) {
}

void Element::render(Renderer &ctx) {
    //get abs x, y;
    screen_x = ctx.context.transform_matrix[2];
    screen_y = ctx.context.transform_matrix[5];
    animation = should_render();
    if (showed) animation.step(pow(1 - 0.2, dt * 60 / 1000));
    else animation.step(1);
    showed = 1;
    float curr_animation = (float) animation;
    visible = curr_animation > 0.01;
    if (visible) {
        animate(ctx);
        on_render(ctx);
        //possibly poll events?
    } else 
        on_render_skip(ctx);

    //event emitter
    if (Ui::focused == this) {
        if (BIT_AT(Input::mouse_buttons_pressed, 0)) {
            focus_state = kMouseDown;
            on_event(kMouseDown);
        }
        else if (BIT_AT(Input::mouse_buttons_released, 0)) {
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

void Element::on_render(Renderer &ctx) {}

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