#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Button.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>
#include <Client/Input.hh>

using namespace Ui;

Element *Ui::make_settings_panel() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Settings"),
        new Ui::HContainer({
            new Ui::ToggleButton(30, &Input::keyboard_movement),
            new Ui::StaticText(16, "Keyboard movement")
        }, 0, 10)
    }, 20, 10, { 
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
    Ui::Panel::settings = elt;
    return elt;
}