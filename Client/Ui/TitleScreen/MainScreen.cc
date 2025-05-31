#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>

using namespace Ui;

Element *Ui::make_title_main_screen() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::StaticText(40, "gardn.clone"),
        new Ui::Element(0, 60),
        new Ui::Choose(
            new Ui::StaticText(30, "Loading..."),
            new Ui::HContainer({
                new Ui::StaticText(30, "Name enter bar"),
                new Ui::Button(90, 40, 
                    new Ui::StaticText(30, "Enter"), 
                    [](uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                    { .fill = 0xff94e873 }
                )
            }, 10, 10, {}),
            [](){ return Game::socket.ready; }
        ),
        new Ui::Element(0,20),
        new Ui::StaticText(16, "florr.io pvp clone"),
        new Ui::StaticText(20, "Mouse to move and attack")
    });
    title->should_render = [](){
        return Game::should_render_title_ui(); //!Game::alive() && Game::on_game_screen == 0;
    };
    return title;
}
