#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

void render_name(Renderer &ctx, Entity const &ent) {
    if (!ent.nametag_visible) return;
    ctx.translate(0, -ent.radius - 18);
    //ctx.center_text_align();
    //ctx.center_text_baseline();
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    ctx.draw_text(ent.name.c_str(), { .size = 18 });
}