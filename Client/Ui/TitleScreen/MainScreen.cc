#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>

using namespace Ui;

Element *Ui::make_title_main_screen() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::StaticText(40, "This is a game name", { .fill = 0xffffffff }),
        new Ui::Choose(
            new Ui::StaticText(30, "Loading...", { .fill = 0xffffffff }),
            new Ui::VContainer({
                new Ui::StaticText(30, "Name enter bar", { .fill = 0xffffffff }),
                new Ui::Button(90, 40, 
                    new Ui::StaticText(30, "Enter"), 
                    [](uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                    { .fill = 0xffcccccc }
                )
            }, 10, 10, {}),
            [](){ return Game::socket.ready; }
        )
    });
    /*new Ui::VContainer({
        new Ui::StaticText(40, "This is a game name", { .fill = 0xffffffff }),
        new Ui::StaticText(30, "Name enter bar", { .fill = 0xffffffff }),
        new Ui::Button(90, 40, 
            new Ui::StaticText(30, "Enter"), 
            [](uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
            { .fill = 0xffcccccc }
        )
    }, 10, 10, {});*/
    title->should_render = [](){
        return !Game::alive() && Game::on_game_screen == 0;
    };
    return title;
}
