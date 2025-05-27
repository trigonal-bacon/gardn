#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Game.hh>
#include <Client/Ui/Extern.hh>
#include <Shared/StaticData.hh>

#include <cstdio>

using namespace Ui;

LevelBar::LevelBar() : Element(300,40) {
    progress = 0;
    should_render = [](){
        return Game::alive();
    };
    style.animate = [&](Element *elt, Renderer &ctx) {
        if (Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::player_id);
            float xp = player.score;
            level = scoreToLevel(xp);
            xp -= levelToScore(level);
            xp /= scoreToPassLevel(level);
            progress.set(xp);
        }
        progress.step(Ui::lerp_amount);
    };
    h_justify = Ui::Element::Left;
    v_justify = Ui::Element::Bottom;
}

void LevelBar::on_render(Renderer &ctx) {
    ctx.set_stroke(0xff222222);
    ctx.round_line_cap();
    ctx.set_line_width(height);
    ctx.begin_path();
    ctx.move_to(-width / 2, 0);
    ctx.line_to(width / 2, 0);
    ctx.stroke();
    ctx.set_stroke(0xff75dd34);
    ctx.set_line_width(height * 0.9);
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