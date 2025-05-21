#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/Container.hh>

using namespace Ui;

Element *Ui::make_death_main_screen() {
    Ui::Element *continue_button = new Ui::Button(
        140,
        40,
        new Ui::StaticText(30, "Continue"),
        [](uint8_t e){ if (e == Ui::kClick) Game::on_game_screen = 0; },
        { .fill = 0xffcccccc }
    );
    continue_button->should_render = [](){
        return !Game::alive() && Game::on_game_screen;
    };
    return continue_button;
}
