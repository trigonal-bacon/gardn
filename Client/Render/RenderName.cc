#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

void render_name(Renderer &ctx, Entity &ent) {
    if (!ent.nametag_visible) return;
    ctx.translate(0, -ent.radius - 18);
    //ctx.center_text_align();
    //ctx.center_text_baseline();
    ctx.draw_text(ent.name.c_str(), { .size = 18 });
}