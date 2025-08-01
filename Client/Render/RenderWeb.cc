#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>
#include <Client/Assets/Assets.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

void render_web(Renderer &ctx, Entity const &ent) {
    //ctx.set_fill(0x20000000);
    //ctx.begin_path();
    //ctx.arc(0,0,ent.radius);
    //ctx.fill();
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    ctx.scale(ent.radius / 10);
    if (ent.team == Game::camera_id)
        ctx.add_color_filter(0x60ffe763, 1);
    draw_web(ctx);
}