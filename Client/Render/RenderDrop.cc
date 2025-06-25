#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void render_drop(Renderer &ctx, Entity const &ent) {
    float animation_value = sinf(Game::timestamp / 100);
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 - ent.deletion_animation);
    ctx.scale(1 + animation_value * 0.03);
    ctx.scale(ent.radius / 30);
    ctx.set_fill(0x40000000);
    ctx.begin_path();
    ctx.round_rect(-36, -36, 72, 72, 5);
    ctx.fill();
    draw_loadout_background(ctx, ent.drop_id);
}