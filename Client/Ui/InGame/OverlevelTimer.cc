#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>

#include <Shared/Map.hh>

using namespace Ui;

OverlevelTimer::OverlevelTimer(float w) : Element(w,w,{}) {}

void OverlevelTimer::on_render(Renderer &ctx) {
    float ratio = Game::overlevel_timer / (PETAL_DISABLE_DELAY * TPS);
    ctx.set_fill(0x80000000);
    ctx.begin_path();
    ctx.arc(0,0,width/2);
    ctx.fill();
    ctx.set_fill(0xffea7f80);
    ctx.begin_path();
    ctx.move_to(0,0);
    ctx.partial_arc(0,0,width/2*0.8,-M_PI/2,-M_PI/2+2*M_PI*ratio,0);
    ctx.close_path();
    ctx.fill();
}

Element *Ui::make_overlevel_indicator() {
    Element *elt = new Ui::HContainer({
        new Ui::VContainer({
            new Ui::StaticText(18, "MOVE RIGHT ->", { .fill = 0xffea7f80, .h_justify = Style::Left }),
            new Ui::StaticText(14, "You are overleveled for this zone", { .fill = 0xffffffff, .h_justify = Style::Left }),
            new Ui::StaticText(14, "You will be unable to attack", { .fill = 0xffffffff, .h_justify = Style::Left }),
        }, 0, 3),
        new OverlevelTimer(50)
    }, 10, 20, {
        .fill = 0x40000000,
        .round_radius = 5,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.set_global_alpha(elt->animation);
        },
        .should_render = [](){
            if (!Game::alive()) return false;
            Entity &player = Game::simulation.get_ent(Game::player_id);
            return Map::get_zone_from_pos(player.x, player.y)->difficulty < Map::difficulty_at_level(score_to_level(player.score))
            && Game::overlevel_timer > 0;
            //return Game::overlevel_timer > 0 ? 1 : 0;
        }
    });
    elt->y = -80;
    return elt;
}