#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

using namespace Ui;

Element *Ui::make_death_main_screen() {
    Ui::Element *continue_button = new Ui::Button(
        145,
        40,
        new Ui::StaticText(28, "Continue"),
        [](Element *elt, uint8_t e){ if (e == Ui::kClick && Game::on_game_screen) Game::on_game_screen = 0; },
        [](){ return !Game::in_game(); },
        {.fill = 0xff94e873, .line_width = 5, .round_radius = 3 }
    );
    Ui::Element *container = new Ui::VContainer({
        new Ui::StaticText(25, "You were killed by"),
        new Ui::DynamicText(30, [](){
            if (!Game::simulation.ent_exists(Game::camera_id))
                return std::string{""};
            if (Game::simulation.get_ent(Game::camera_id).killed_by == "") 
                return std::string{"a mysterious entity"};
            return Game::simulation.get_ent(Game::camera_id).killed_by;
        }),
        new Ui::Element(0,100),
        continue_button,
        new Ui::StaticText(14, "(or press ENTER to continue)")
    }, 0, 10, { .animate = [](Element *elt, Renderer &ctx) {
        ctx.translate(0, (elt->animation - 1) * ctx.height * 0.6);
    }, .should_render = [](){ return !Game::alive() && Game::should_render_game_ui(); } });
    return container;
}
