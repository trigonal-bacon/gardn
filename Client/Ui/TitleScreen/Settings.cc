#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Button.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>
#include <Client/Input.hh>

using namespace Ui;

std::unique_ptr<Element> Ui::make_settings_panel() {
    auto elt = el<VContainer>(make_children(
        el<StaticText>(25, "Settings"),
        el<HContainer>(make_children(
            el<ToggleButton>(30, &Input::keyboard_movement),
            el<StaticText>(16, "Keyboard movement")
        ), 0, 10, Style{.h_justify = Style::Left }),
        el<HContainer>(make_children(
            el<ToggleButton>(30, &Input::movement_helper),
            el<StaticText>(16, "Movement helper")
        ), 0, 10, Style{.h_justify = Style::Left }),
        el<HContainer>(make_children(
            el<ToggleButton>(30, &Game::show_debug),
            el<StaticText>(16, "Debug stats")
        ), 0, 10, Style{.h_justify = Style::Left }),
        el<StaticText>(12, "Made by bismuth (trigonal-bacon)"),
        el<StaticText>(12, "Asset credits: M28 and affiliates")
    ), 20, 10, Style{ 
        .fill = 0xff5a9fdb,
        .line_width = 7,
        .round_radius = 3,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.translate(0, (1 - elt->animation) * 2 * elt->height);
        },
        .should_render = [](){
            return Ui::panel_open == Panel::kSettings && Game::should_render_title_ui();
        },
        .h_justify = Style::Left,
        .v_justify = Style::Bottom
    });
    Ui::Panel::settings = elt.get();
    return elt;
}