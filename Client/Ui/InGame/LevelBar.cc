#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Game.hh>
#include <Client/Ui/Extern.hh>
#include <Shared/StaticData.hh>

#include <cstdio>
#include <string>

using namespace Ui;

LevelBar::LevelBar() : Element(300,40) {
    progress = 0;
    style.animate = [&](Element *elt, Renderer &ctx) {
        if (Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::player_id);
            float xp = player.score;
            level = score_to_level(xp);
            xp -= level_to_score(level);
            xp /= score_to_pass_level(level);
            progress.set(xp);
        }
        progress.step(Ui::lerp_amount);
    };
    style.h_justify = Style::Left;
    style.v_justify = Style::Bottom;
}

void LevelBar::on_render(Renderer &ctx) {
    ctx.set_stroke(0xff222222);
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
    ctx.set_text_size(height / 2);
    ctx.set_line_width(height / 2 * 0.12);
    ctx.center_text_align();
    ctx.center_text_baseline();
    ctx.set_stroke(0xff222222);
    ctx.set_fill(0xffffffff);
    char text[16];
    std::snprintf(text, 15, "Level %d", level);
    ctx.stroke_text(text);
    ctx.fill_text(text);
}

Element *Ui::make_level_bar() {
    Element *level_bar = new Ui::VContainer({
        new Ui::LevelBar(),
        new Ui::DynamicText(12, [](){
            std::string format_string{};
            if (Game::alive()) {
                Entity &player = Game::simulation.get_ent(Game::player_id);
                uint32_t level = score_to_level(player.score);
                if (level < 45)
                    format_string = std::format("Extra petal slot at level {}", div_round_up(level + 1, 15) * 15);
            }
            return format_string;
        }),
        new Ui::Element(1,60)
    }, 0, 5, {});
    level_bar->should_render = [](){
        return Game::alive();
    };
    level_bar->style.h_justify = Style::Left;
    level_bar->style.v_justify = Style::Bottom;
    level_bar->style.animate = [](Element *elt, Renderer &ctx) {
        ctx.translate(-(1 - (float) elt->animation) * 1.5 * elt->width, 0);
    };
    return level_bar;
}