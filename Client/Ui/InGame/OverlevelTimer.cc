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

std::unique_ptr<Element> Ui::make_overlevel_indicator() {
    auto elt = el<HContainer>(make_children(
        el<VContainer>(make_children(
            el<StaticText>(18, "MOVE RIGHT →", Style{ .fill = 0xffea7f80, .h_justify = Style::Left }),
            el<StaticText>(14, "You are overleveled for this zone", Style{ .fill = 0xffffffff, .h_justify = Style::Left }),
            el<StaticText>(14, "You will be unable to attack", Style{ .fill = 0xffffffff, .h_justify = Style::Left })
        ), 0, 3),
        el<OverlevelTimer>(50)
    ), 10, 20, Style{
        .fill = 0x40000000,
        .round_radius = 5,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.set_global_alpha(elt->animation);
        },
        .should_render = [](){
            if (!Game::alive()) return false;
            Entity const &player = Game::simulation.get_ent(Game::player_id);
            return MAP_DATA[Map::get_zone_from_pos(player.get_x(), player.get_y())].difficulty < Map::difficulty_at_level(score_to_level(Game::score))
            && Game::overlevel_timer > 0;
        }
    });
    elt->y = -80;
    return elt;
}