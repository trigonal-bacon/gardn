#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <Client/Game.hh>

#include <cmath>

void render_mob(Renderer &ctx, Entity &ent) {
    ctx.set_global_alpha(1 - ent.deletion_tick * 0.2);
    ctx.scale(1 + 0.1 * ent.deletion_tick);
    ctx.add_color_filter(0xffac0000, ent.damaged / 2);
    draw_static_mob(ent.mob_id, ctx, ent.animation, ent.radius);
}