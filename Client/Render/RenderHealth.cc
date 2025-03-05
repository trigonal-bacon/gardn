#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

void render_health(Renderer &ctx, Entity &ent) {
    if (ent.has_component(kPetal)) return;
    //if (ent.healthbar_opacity < 0.01) return;
    ctx.set_global_alpha(1 - ent.deletion_tick * 0.2);
    ctx.scale(1 + 0.1 * ent.deletion_tick);
    ctx.translate(0, ent.radius * 1.2 + 10);
    ctx.round_line_cap();
    ctx.set_stroke(0xff222222);
    ctx.set_line_width(10);
    ctx.begin_path();
    ctx.move_to(-ent.radius, 0);
    ctx.line_to(ent.radius, 0);
    ctx.stroke();
    ctx.set_stroke(0xff75dd34);
    ctx.set_line_width(7);
    ctx.begin_path();
    ctx.move_to(-ent.radius, 0);
    ctx.line_to(-ent.radius + 2 * ent.radius * ent.health_ratio, 0);
    ctx.stroke();
}