#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Game.hh>
#include <Client/Ui/Extern.hh>
#include <Shared/StaticData.hh>

#include <format>
#include <string>

using namespace Ui;

LevelBar::LevelBar() : Element(300,40) {
    progress = 0;
    style.animate = [&](Element *elt, Renderer &ctx) {
        if (Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::player_id);
            float xp = Game::score;
            level = score_to_level(xp);
            xp -= level_to_score(level);
            xp = fclamp(xp / score_to_pass_level(level), 0, 1);
            progress.set(xp);
        }
        progress.step(Ui::lerp_amount);
    };
    style.h_justify = Style::Left;
    style.v_justify = Style::Bottom;
}

void LevelBar::on_render(Renderer &ctx) {
    ctx.set_stroke(0xc0222222);
    ctx.round_line_cap();
    ctx.set_line_width(height);
    ctx.begin_path();
    ctx.move_to(-width / 2, 0);
    ctx.line_to(width / 2, 0);
    ctx.stroke();
    ctx.set_stroke(0xfff9e496);
    ctx.set_line_width(height * 0.8);
    ctx.begin_path();
    ctx.move_to(-width / 2, 0);
    ctx.line_to(-width / 2 + width * ((float) progress), 0);
    ctx.stroke();

    std::string text = "Lvl " + std::to_string(level) + " Flower";
    ctx.draw_text(text.c_str(), { .size = 16 });
    ctx.translate(0, -height/2 - 16);
    ctx.draw_text(Game::nickname.c_str(), { .size = 24 });
}

Element *Ui::make_level_bar() {
    Element *level_bar = new Ui::VContainer({
        new Ui::LevelBar(),
        new Ui::DynamicText(14, [](){
            std::string format_string;
            if (Game::alive()) {
                Entity &player = Game::simulation.get_ent(Game::player_id);
                uint32_t level = score_to_level(Game::score);
                if (loadout_slots_at_level(level) < MAX_SLOT_COUNT) {
                    level = div_round_up(level + 1, LEVELS_PER_EXTRA_SLOT) * LEVELS_PER_EXTRA_SLOT;
                    if (level > MAX_LEVEL) level = MAX_LEVEL;
                    format_string = std::format("Extra petal slot at level {}", level);
                }
            }
            return format_string;
        }),
        new Ui::Element(0,65)
    }, 0, 5, { .should_render = []() { return Game::alive(); } });
    level_bar->style.h_justify = Style::Left;
    level_bar->style.v_justify = Style::Bottom;
    level_bar->style.animate = [](Element *elt, Renderer &ctx) {
        ctx.translate(-(1 - (float) elt->animation) * 1.5 * elt->width, 0);
    };
    return level_bar;
}