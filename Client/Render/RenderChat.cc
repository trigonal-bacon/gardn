#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

void render_chat(Renderer &ctx, Entity const &ent) {
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    float height = 18;
    ctx.set_text_size(height);
    float width = ctx.get_text_size(ent.get_text().c_str());
    ctx.set_stroke((0xc0 << 24) | (FLOWER_COLORS[ent.get_color()] & 0xffffff));
    ctx.set_line_width(height + 6);
    ctx.round_line_cap();
    ctx.begin_path();
    ctx.move_to(-width / 2, 0);
    ctx.line_to(width / 2, 0);
    ctx.stroke();
    ctx.draw_text(ent.get_text().c_str(), { .size = height });
}
