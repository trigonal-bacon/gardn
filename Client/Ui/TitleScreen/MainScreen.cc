#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/TextInput.hh>

using namespace Ui;

Element *Ui::make_title_main_screen() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::StaticText(40, "gardn.clone"),
        new Ui::Element(0, 60),
        new Ui::Choose(
            new Ui::StaticText(30, "Loading..."),
            new Ui::VContainer({
                new Ui::StaticText(20, "This pretty little flower is called..."),
                new Ui::HContainer({
                    new Ui::TextInput("t0", 350, 40, 16, { 
                        .animate = [](Element *elt, Renderer &ctx) { 
                            ctx.set_global_alpha((float) elt->animation);
                            ctx.translate(0, ((float) elt->animation - 1) * ctx.height * 0.6);
                        },
                        .should_render = [](){
                            return !Game::in_game() && Game::transition_circle == 0;
                        }
                    }),
                    new Ui::Button(110, 40, 
                        new Ui::StaticText(28, "Spawn"), 
                        [](uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                        { .fill = 0xff94e873, .round_radius = 7 }
                    )
                }, 0, 10, {}),
            }, 10, 5),
            [](){ return Game::socket.ready; }
        ),
        new Ui::Element(0,20),
        new Ui::StaticText(16, "florr.io pvp clone"),
        new Ui::StaticText(20, "Mouse to move and attack")
    }, 0, 0, { .should_render = [](){ return Game::should_render_title_ui(); } });

    return title;
}
