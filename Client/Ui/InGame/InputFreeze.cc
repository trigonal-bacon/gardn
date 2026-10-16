#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Game.hh>
#include <Client/Input.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>

#include <cmath>

using namespace Ui;

static void opacity_animate(Element& elt, Renderer& ctx) {
  ctx.set_global_alpha((float)elt.animation);
}

InputFreeze::InputFreeze()
    : Container(
          make_children(
              el<VContainer>(make_children(el<StaticText>(
                                 14, "Move mouse here to disable movement")),
                             10, 0,
                             Style{.animate = opacity_animate,
                                   .should_render =
                                       []() { return !Input::freeze_input; }}),
              el<VContainer>(
                  make_children(el<StaticText>(
                      14, "You can also use [Q] and [E] to cycle secondaries")),
                  10, 0,
                  Style{
                      .animate = opacity_animate,
                      .should_render = []() { return Input::freeze_input; }})),
          0, 0, Style{}) {
  width = render_width = children[0]->width;
  height = render_height = children[0]->height;
  style.animate = [&](Element& elt, Renderer& ctx) {
    if (!Game::alive())
      Input::freeze_input = 0;
    if (auto p = parent.get(); p) {
      if (Input::freeze_input) {
        render_width = lerp(render_width, p->width + 40, Ui::lerp_amount * 1.5);
        render_height =
            lerp(render_height, p->height + 20, Ui::lerp_amount * 1.5);
      } else {
        render_width =
            lerp(render_width, children[0]->width, Ui::lerp_amount * 1.5);
        render_height =
            lerp(render_height, children[0]->height, Ui::lerp_amount * 1.5);
      }
    }
    if ((fabsf(Input::mouse_x - screen_x) > render_width * Ui::scale / 2 ||
         Ui::window_height - Input::mouse_y > render_height * Ui::scale) &&
        Ui::UiLoadout::num_petals_selected == 0)
      Input::freeze_input = 0;
  };
  style.round_radius = 10;
  style.should_render = []() {
    return !Input::keyboard_movement && !Input::is_mobile;
  };
}

void InputFreeze::on_render(Renderer& ctx) {
  ctx.set_fill(0x80000000);
  ctx.begin_path();
  ctx.round_rect(-render_width / 2, -render_height + height / 2, render_width,
                 render_height + style.round_radius, style.round_radius);
  ctx.fill();
  Container::on_render(ctx);
}

void InputFreeze::on_render_skip(Renderer& ctx) {
  Container::on_render_skip(ctx);
}

void InputFreeze::on_event(uint8_t event) {
  if (event == Ui::kMouseHover || event == Ui::kMouseMove)
    Input::freeze_input = 1;
}

void InputFreeze::poll_events(const ScreenEvent& event) {
  Element::poll_events(event);
}
