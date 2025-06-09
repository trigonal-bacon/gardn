#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <Client/Game.hh>

#include <cmath>

void render_mob(Renderer &ctx, Entity const &ent) {
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    if (ent.damage_flash > 0.75)
        ctx.add_color_filter(0xffffffff, ent.damage_flash * 0.8);
    else if (ent.damage_flash > 0.01)
        ctx.add_color_filter(0xffac0000, ent.damage_flash * 1.5);
    uint32_t flags = 0;
    flags |= (ent.team == Game::camera_id);
    flags |= (ent.is_tail << 1);
    draw_static_mob(ent.mob_id, ctx, {ent.animation, ent.radius, ent.id.id, flags});
    if (ent.deletion_animation > 0)
        Game::seen_mobs[ent.mob_id] = 1;
}