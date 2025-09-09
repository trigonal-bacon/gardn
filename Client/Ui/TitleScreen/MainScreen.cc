#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Button.hh>
#include <Client/Ui/Choose.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Ui/TextInput.hh>

#include <Client/Debug.hh>
#include <Client/DOM.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>

#include <Shared/Config.hh>

#include <format>

using namespace Ui;

Element *Ui::make_title_input_box() {
    Ui::Element *title = new Ui::VContainer({
        new Ui::Element(0, 60),
        new Ui::Choose(
            new Ui::StaticText(40, "Connecting..."),
            new Ui::VContainer({
                new Ui::StaticText(20, "This pretty little flower is called..."),
                new Ui::HContainer({
                    new Ui::TextInput(Game::nickname, 400, 50, MAX_NAME_LENGTH, {
                        .line_width = 5,
                        .round_radius = 5,
                        .animate = [](Element *elt, Renderer &ctx) { 
                            ctx.translate(0, (elt->animation - 1) * ctx.height * 0.6);
                        },
                        .should_render = [](){
                            return !Game::in_game() && Game::transition_circle <= 0;
                        }
                    }),
                    new Ui::Button(110, 36, 
                        new Ui::StaticText(25, "Spawn"), 
                        [](Element *elt, uint8_t e){ if (e == Ui::kClick) Game::spawn_in(); },
                        [](){ return Game::in_game() != 0; },
                        { .fill = 0xff1dd129, .line_width = 5, .round_radius = 3 }
                    )
                }, 0, 10,{}),
                new Ui::StaticText(14, "(or press ENTER to spawn)")
            }, 10, 5, { .animate = [](Element *elt, Renderer &ctx) {
                ctx.translate(0, (elt->animation - 1) * ctx.height);
            } }),
            [](){ return Game::socket.ready; }
        ),
        new Ui::Element(0,20),
        new Ui::StaticText(16, "open-source florr.io pvp clone"),
    }, 0, 0, { .animate = [](Element *elt, Renderer &ctx){}, .should_render = [](){ return Game::should_render_title_ui(); } });
    return title;
}

Element *Ui::make_title_info_box() {
    Element *elt = new Ui::Choose(
        new Ui::Choose(
            new Ui::VContainer({
                new Ui::StaticText(35, "How to play"),
                new Ui::Element(0,5),
                new Ui::StaticText(16, "Use mouse to move"),
                new Ui::StaticText(16, "Right click to attack"),
                new Ui::StaticText(16, "Left click to defend")
            }, 0, 5, { .no_animation = 1 }),
            new Ui::VContainer({
                new Ui::StaticText(35, "How to play"),
                new Ui::Element(0,5),
                new Ui::StaticText(16, "Use WASD or arrow keys to move"),
                new Ui::StaticText(16, "SPACE to attack"),
                new Ui::StaticText(16, "SHIFT to defend")
            }, 0, 5, { .no_animation = 1 }),
            [](){
                return Input::keyboard_movement;
            }, { .no_polling = 1 }
        ),
        new Ui::VContainer({
            new Ui::HContainer({
                new Ui::DynamicText(16, [](){
                    return std::format("You will respawn at level {}", Game::respawn_level);
                }, { .fill = 0xffffffff, .no_animation = 1 }),
                new Ui::StaticText(16, " with:", {
                    .fill = 0xffffffff,
                    .should_render = [](){
                        if (!Game::simulation.ent_exists(Game::camera_id)) return false;
                        return Game::simulation.get_ent(Game::camera_id).get_inventory(0) > PetalID::kBasic;
                    }
                })
            }, 0, 0),
            new Ui::HContainer(
                Ui::make_range(0, MAX_SLOT_COUNT, [](uint32_t i){ return (Element *) (new Ui::TitlePetalSlot(i)); })
            , 0, 10),
            new Ui::HContainer({
                Ui::make_range(MAX_SLOT_COUNT, 2*MAX_SLOT_COUNT, [](uint32_t i){ return (Element *) (new Ui::TitlePetalSlot(i)); })
            }, 0, 10)
        }, 0, 10, { .no_animation = 1 }),
        [](){
            return Game::respawn_level > 1 ? 1 : 0;
        }
    );
    elt->style.animate = [](Element *elt, Renderer &ctx) {
        elt->x = 0;
        elt->y = 270;
    };
    return elt;
}


Element *Ui::make_panel_buttons() {
   Element *elt = new Ui::HContainer({
        new Ui::Button(100, 35, 
            new Ui::StaticText(16, "Settings"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kSettings) {
                    Ui::panel_open = Panel::kSettings;
                    Element *pg = Ui::Panel::settings;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kSettings; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(100, 35, 
            new Ui::HContainer({
                new Ui::Element(0,0),
                new Ui::StaticText(16, "Petals"),
                new Ui::PetalsCollectedIndicator(20)
            }, 0, 10),
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kPetals) {
                    Ui::panel_open = Panel::kPetals;
                    Element *pg = Ui::Panel::petal_gallery;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kPetals; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(100, 35, 
            new Ui::StaticText(16, "Mobs"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kMobs) {
                    Ui::panel_open = Panel::kMobs;
                    Element *pg = Ui::Panel::mob_gallery;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kMobs; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
        new Ui::Button(120, 35, 
            new Ui::StaticText(16, "Changelog"), 
            [](Element *elt, uint8_t e){ if (e == Ui::kClick) {
                if (Ui::panel_open != Panel::kChangelog) {
                    Ui::panel_open = Panel::kChangelog;
                    Element *pg = Ui::Panel::changelog;
                    pg->x = elt->screen_x / Ui::scale - pg->width / 2;
                    pg->y = -(elt->height + 20);
                    if (pg->x < 10) 
                        pg->x = 10;
                }
                else Ui::panel_open = Panel::kNone;
            } },
            [](){ return Ui::panel_open == Panel::kChangelog; },
            { .fill = 0xff5a9fdb, .line_width = 5, .round_radius = 3 }
        ),
   }, 10, 10, { .should_render = [](){ return Game::should_render_title_ui(); }, .h_justify = Style::Left, .v_justify = Style::Bottom });
   return elt;
}

Element *Ui::make_debug_stats() {
    Element *elt = new Ui::VContainer({
        new Ui::DynamicText(12, [](){
            float pxls = 0;
            for (Renderer *r : Renderer::renderers) pxls += r->width * r->height;
            return std::format("{} ctxs - {} pxls - {:.0f}x{:.0f}", Renderer::renderers.size(), format_score(pxls), Ui::window_width, Ui::window_height);
        }, { .fill = 0xffffffff, .h_justify = Style::Right }),
        new Ui::DynamicText(12, [](){
            if (Debug::frame_times.size() == 0) return std::string{"Failed to show debug stats"};
            float min_dt = Debug::tick_times[0];
            float avg_dt = min_dt;
            float max_dt = min_dt;
            for (uint32_t i = 1; i < Debug::tick_times.size(); ++i) {
                float const frame_time = Debug::tick_times[i];
                if (min_dt > frame_time) min_dt = frame_time;
                else if (max_dt < frame_time) max_dt = frame_time;
                avg_dt += frame_time;
            }
            avg_dt /= Debug::tick_times.size();
            return std::format("tick: {:.1f}/{:.1f}/{:.1f} ms (min/avg/max)", min_dt, avg_dt, max_dt, 1000 / Ui::dt);
        }, { .fill = 0xffffffff, .h_justify = Style::Right }),
        new Ui::DynamicText(12, [](){
            if (Debug::frame_times.size() == 0) return std::string{"Failed to show debug stats"};
            float min_dt = Debug::frame_times[0];
            float avg_dt = min_dt;
            float max_dt = min_dt;
            for (uint32_t i = 1; i < Debug::frame_times.size(); ++i) {
                float const frame_time = Debug::frame_times[i];
                if (min_dt > frame_time) min_dt = frame_time;
                else if (max_dt < frame_time) max_dt = frame_time;
                avg_dt += frame_time;
            }
            avg_dt /= Debug::frame_times.size();
            return std::format("frame: {:.1f}/{:.1f}/{:.1f} ms (min/avg/max) - {:.1f} fps", min_dt, avg_dt, max_dt, 1000 / Ui::dt);
        }, { .fill = 0xffffffff, .h_justify = Style::Right })
    }, 5, 5, { .should_render = [](){ return Game::show_debug; }, .h_justify = Style::Right, .v_justify = Style::Bottom, .no_animation = 1 });
    return elt;
}

Element *Ui::make_github_link_button() {
    Element *elt = new Ui::HContainer({
        new Ui::Button(50, 50, 
            new Ui::StaticIcon([](Element *elt, Renderer &ctx){
                ctx.scale(elt->width / 20);
                ctx.set_fill(0xfff1f1f1);
                ctx.begin_path();
                ctx.move_to(0.00, -10.00);
                ctx.bcurve_to(5.52, -10.00, 10.00, -5.41, 10.00, 0.25);
                ctx.bcurve_to(10.00, 4.78, 7.14, 8.62, 3.17, 9.98);
                ctx.bcurve_to(2.66, 10.08, 2.48, 9.76, 2.48, 9.49);
                ctx.bcurve_to(2.48, 9.15, 2.49, 8.05, 2.49, 6.67);
                ctx.bcurve_to(2.49, 5.72, 2.17, 5.09, 1.81, 4.78);
                ctx.bcurve_to(4.04, 4.52, 6.38, 3.66, 6.38, -0.28);
                ctx.bcurve_to(6.38, -1.40, 5.99, -2.32, 5.35, -3.03);
                ctx.bcurve_to(5.45, -3.29, 5.80, -4.34, 5.25, -5.75);
                ctx.bcurve_to(5.25, -5.75, 4.41, -6.02, 2.50, -4.70);
                ctx.bcurve_to(1.71, -4.92, 0.85, -5.04, 0.00, -5.04);
                ctx.bcurve_to(-0.85, -5.04, -1.71, -4.92, -2.50, -4.70);
                ctx.bcurve_to(-4.41, -6.02, -5.25, -5.75, -5.25, -5.75);
                ctx.bcurve_to(-5.80, -4.34, -5.45, -3.29, -5.35, -3.03);
                ctx.bcurve_to(-5.99, -2.32, -6.38, -1.40, -6.38, -0.28);
                ctx.bcurve_to(-6.38, 3.65, -4.05, 4.53, -1.82, 4.79);
                ctx.bcurve_to(-2.11, 5.04, -2.37, 5.49, -2.46, 6.16);
                ctx.bcurve_to(-3.03, 6.42, -4.48, 6.87, -5.37, 5.30);
                ctx.bcurve_to(-5.37, 5.30, -5.90, 4.32, -6.90, 4.25);
                ctx.bcurve_to(-6.90, 4.25, -7.88, 4.23, -6.97, 4.87);
                ctx.bcurve_to(-6.97, 4.87, -6.32, 5.18, -5.86, 6.37);
                ctx.bcurve_to(-5.86, 6.37, -5.27, 8.20, -2.49, 7.58);
                ctx.bcurve_to(-2.49, 8.44, -2.48, 9.24, -2.48, 9.49);
                ctx.bcurve_to(-2.48, 9.76, -2.66, 10.08, -3.16, 9.98);
                ctx.bcurve_to(-7.13, 8.63, -10.00, 4.78, -10.00, 0.25);
                ctx.bcurve_to(-10.00, -5.41, -5.52, -10.00, 0.00, -10.00);
                ctx.fill();
            }, 40, 40),
            [](Element *elt, uint8_t e){
                if (e == Ui::kClick) DOM::open_page("https://github.com/trigonal-bacon/gardn/");
            },
            nullptr,
            { .fill = 0xff333333, .line_width = 4, .round_radius = 4 }
        )
    }, 10, 10, { .h_justify = Style::Right, .v_justify = Style::Top });
    return elt;
}