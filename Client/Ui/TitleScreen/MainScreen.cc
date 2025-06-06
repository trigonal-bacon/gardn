#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Ui/TextInput.hh>

using namespace Ui;

Element *Ui::make_title_main_screen() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::StaticText(60, "gardn.clone"),
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
                    new Ui::Button(110, 36, 
                        new Ui::StaticText(25, "Spawn"), 
                        [](Element *elt, uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                        { .fill = 0xff94e873, .line_width = 5, .round_radius = 3 }
                    )
                }, 0, 10, {}),
            }, 10, 5),
            [](){ return Game::socket.ready; }
        ),
        new Ui::Element(0,20),
        new Ui::StaticText(16, "florr.io pvp clone"),
        new Ui::StaticText(20, "Mouse to move and attack")
    }, 0, 0, { .animate = [](Element *elt, Renderer &ctx){}, .should_render = [](){ return Game::should_render_title_ui(); } });

    return title;
}


Element *Ui::make_panel_buttons() {
   Element *elt = new Ui::HContainer({
        new Ui::Button(100, 30, 
            new Ui::StaticText(16, "Settings"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kSettings) {
                    Ui::panel_open = Panel::kSettings;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(90, 30, 
            new Ui::StaticText(16, "Petals"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kPetals) {
                    Ui::panel_open = Panel::kPetals;
                    Element *pg = Ui::Panel::petal_gallery;
                    pg->x = elt->screen_x - Ui::scale * pg->width / 2;
                    pg->y = -Ui::scale * (elt->height + 15);
                    if (pg->x < 10 * Ui::scale) 
                        pg->x = 10 * Ui::scale;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(80, 30, 
            new Ui::StaticText(16, "Mobs"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) e = Ui::kClick; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(110, 30, 
            new Ui::StaticText(16, "Changelog"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) e = Ui::kClick; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
   }, 10, 10, { .should_render = [](){ return Game::should_render_title_ui(); }, .h_justify = Style::Left, .v_justify = Style::Bottom });
   return elt;
}