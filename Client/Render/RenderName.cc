#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

void render_name(Renderer &ctx, Entity &ent) {
    if (!ent.nametag_visible) return;
    float text_size = 18;
    ctx.set_text_size(text_size);
    ctx.set_line_width(text_size * 0.12);
    ctx.center_text_align();
    ctx.center_text_baseline();
    ctx.set_fill(0xffffffff);
    ctx.set_stroke(0xff222222);
    ctx.begin_path();
    char const *name_str = ent.name.c_str();
    ctx.translate(0, -ent.radius - text_size);
    ctx.stroke_text(name_str);
    ctx.fill_text(name_str);
}