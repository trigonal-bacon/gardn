#include <Client/Ui/InGame/GameInfo.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>
#include <Client/Game.hh>

#include <Shared/Map.hh>

using namespace Ui;

Minimap::Minimap(float w) : Element(w, w*ARENA_HEIGHT/ARENA_WIDTH, {}) {}

void Minimap::on_render(Renderer &ctx) {
    ctx.set_line_width(7);
    ctx.set_stroke(0xff444444);
    ctx.stroke_rect(-width/2,-height/2,width,height);
    ctx.translate(-width/2,-height/2);
    ctx.scale(width/ARENA_WIDTH);
    for (ZoneDefinition const &def : MAP_DATA) {
        ctx.set_fill(def.color);
        ctx.fill_rect(def.left, def.top, def.right - def.left, def.bottom - def.top);
        ctx.translate((def.left+def.right)/2,(def.top+def.bottom)/2);
        ctx.draw_text(def.name, { .size = (def.bottom-def.top)/2 });
        ctx.translate(-(def.left+def.right)/2,-(def.top+def.bottom)/2);
    }
    if (!Game::simulation.ent_exists(Game::camera_id)) return;
    Entity const &camera = Game::simulation.get_ent(Game::camera_id);
    Game::simulation.for_each<kDot>([&](Simulation *sim, Entity const &ent){
        if (ent.get_parent() == camera.get_player()) return;
        RenderContext c(&ctx);
        ctx.translate(ent.get_x(), ent.get_y());
        ctx.set_global_alpha(1 - ent.deletion_animation);
        ctx.scale(1 + 0.5 * ent.deletion_animation);
        ctx.set_fill(FLOWER_COLORS[ent.get_color()]);
        ctx.set_stroke(Renderer::HSV(FLOWER_COLORS[ent.get_color()], 0.8));
        ctx.set_line_width(ARENA_WIDTH / 120);
        ctx.begin_path();
        ctx.arc(0, 0, ARENA_WIDTH / 40);
        ctx.fill();
        ctx.stroke();
    });
    ctx.set_fill(0xffffe763);
    ctx.set_stroke(Renderer::HSV(0xffffe763, 1.0));
    ctx.set_line_width(ARENA_WIDTH / 120);
    ctx.begin_path();
    ctx.arc(camera.get_camera_x(), camera.get_camera_y(), ARENA_WIDTH / 40);
    ctx.fill();
    ctx.stroke();
}

Element *Ui::make_minimap() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Map"),
        new Ui::Minimap(300)
    }, 20, 10, { 
        .should_render = [](){ return Game::should_render_game_ui(); },
        .h_justify = Style::Right,
        .v_justify = Style::Bottom
    });
    elt->y = -75;
    return elt;
}