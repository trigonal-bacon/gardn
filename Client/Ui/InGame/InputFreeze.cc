#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>
#include <Client/Input.hh>

#include <cmath>

using namespace Ui;

static void opacity_animate(Element *elt, Renderer &ctx) {
    ctx.set_global_alpha((float) elt->animation);
}

InputFreeze::InputFreeze() : Choose(
    new Ui::VContainer({new Ui::StaticText(14, "Move mouse here to disable movement")}, 10, 0, { .animate = opacity_animate }),
    new Ui::VContainer({new Ui::StaticText(14, "You cannot use the keys")}, 10, 0, { .animate = opacity_animate }),
    [](){ return Input::freeze_input; }) {
    render_width = first->width;
    render_height = first->height;
    style.animate = [&](Element *elt, Renderer &ctx){
        if (Input::freeze_input) {
            LERP(render_width, parent->width, Ui::lerp_amount);
            LERP(render_height, parent->height, Ui::lerp_amount);
        } else {
            LERP(render_width, first->width, Ui::lerp_amount);
            LERP(render_height, first->height, Ui::lerp_amount);
        }
        if ((fabsf(Input::mouse_x - screen_x) > render_width * Ui::scale / 2 ||
            Ui::window_height - Input::mouse_y > render_height * Ui::scale) &&
            Ui::UiLoadout::petal_selected == nullptr)
            Input::freeze_input = 0;
    };
    style.round_radius = 6;
    //style.fill = 0x80000000;
}

void InputFreeze::on_render(Renderer &ctx) {
    {
        ctx.set_fill(0x80000000);
        ctx.begin_path();
        ctx.round_rect(-render_width / 2, -render_height + height / 2, render_width, render_height, style.round_radius);
        ctx.fill();
    }
    Choose::on_render(ctx);
}

void InputFreeze::on_render_skip(Renderer &ctx) {
    Choose::on_render_skip(ctx);
}

void InputFreeze::on_event(uint8_t event) {
    if (event == Ui::kMouseHover || event == Ui::kMouseMove) 
        Input::freeze_input = 1;
}

void InputFreeze::poll_events() {
    Element::poll_events();
}