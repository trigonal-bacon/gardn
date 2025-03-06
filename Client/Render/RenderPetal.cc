#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

void render_petal(Renderer &ctx, Entity &ent) {
    ctx.set_global_alpha(1 - ent.deletion_tick * 0.2);
    ctx.scale(1 + 0.1 * ent.deletion_tick);
    ctx.add_color_filter(0xffac0000, ent.damage_flash * 0.8);
    draw_static_petal_single(ent.petal_id, ctx);
}