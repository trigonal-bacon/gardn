#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Game.hh>
#include <Client/Ui/Button.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>

#include <Client/Assets/Assets.hh>

#include <cmath>

using namespace Ui;

DeadFlowerIcon::DeadFlowerIcon(float width) : Element(width, width, {}) {}

void DeadFlowerIcon::on_render(Renderer &ctx) {
    if (Game::loadout_count == 0) return;
    ctx.scale((width / 3) / 25);
    for (uint32_t i = 0; i < Game::loadout_count; ++i) {
        float angle = 2 * M_PI * i / Game::loadout_count + 0.5;
        float offset_x = cosf(angle) * 40;
        float offset_y = sinf(angle) * 25;
        if (offset_y > 0) continue;
        RenderContext c(&ctx);
        ctx.translate(offset_x, offset_y);
        ctx.rotate(PETAL_DATA[Game::cached_loadout[i]].attributes.icon_angle);
        draw_static_petal_single(Game::cached_loadout[i], ctx);
    }
    {
        RenderContext c(&ctx);
        ctx.rotate(-0.2);
        float flower_radius = width / 3;
        draw_static_flower(ctx, { .radius = 25, .mouth = 5, .flags = 1<<1 });
    }
    for (uint32_t i = 0; i < Game::loadout_count; ++i) {
        float angle = 2 * M_PI * i / Game::loadout_count + 0.5;
        float offset_x = cosf(angle) * 40;
        float offset_y = sinf(angle) * 25;
        if (offset_y <= 0) continue;
        RenderContext c(&ctx);
        ctx.translate(offset_x, offset_y);
        ctx.rotate(PETAL_DATA[Game::cached_loadout[i]].attributes.icon_angle);
        draw_static_petal_single(Game::cached_loadout[i], ctx);
    }
}

Element *Ui::make_death_main_screen() {
    Ui::Element *continue_button = new Ui::Button(
        145,
        40,
        new Ui::StaticText(28, "Continue"),
        [](Element *elt, uint8_t e){ if (e == Ui::kClick && Game::on_game_screen) Game::on_game_screen = 0; },
        [](){ return !Game::in_game(); },
        {.fill = 0xff1dd129, .line_width = 5, .round_radius = 3 }
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
        new Ui::DeadFlowerIcon(100),
        continue_button,
        new Ui::StaticText(14, "(or press ENTER to continue)")
    }, 0, 10, { .animate = [](Element *elt, Renderer &ctx) {
        ctx.translate(0, (elt->animation - 1) * ctx.height * 0.6);
    }, .should_render = [](){ return !Game::alive() && Game::should_render_game_ui(); } });
    return container;
}
