#include <Client/Render/RenderEntity.hh>

#include <Client/Game.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

void render_health(Renderer &ctx, Entity &ent) {
    if (ent.has_component(kPetal)) return;
    if (ent.has_component(kMob)) return;
    if (ent.damaged)
        ent.last_damaged_time = Game::timestamp;
    if ((float) ent.health_ratio > 0.999)
        LERP(ent.healthbar_opacity, 0, Ui::lerp_amount)
    else
        ent.healthbar_opacity = 1;
    if (ent.healthbar_lag < ent.health_ratio)
        ent.healthbar_lag = ent.health_ratio;
    else if (Game::timestamp - ent.last_damaged_time > 250)
        LERP(ent.healthbar_lag, ent.health_ratio, Ui::lerp_amount / 3)
    if (ent.healthbar_opacity < 0.01) return;
    float w = ent.radius * 1.25;
    ctx.set_global_alpha(1 - ent.deletion_tick * 0.2);
    ctx.scale(1 + 0.1 * ent.deletion_tick);
    ctx.translate(-w, ent.radius * 1.25 + 10);
    ctx.round_line_cap();
    ctx.set_stroke(0xff222222);
    ctx.set_line_width(10);
    ctx.begin_path();
    ctx.move_to(0, 0);
    ctx.line_to(2 * w, 0);
    ctx.stroke();
    if (ent.healthbar_lag > ent.health_ratio) {
        ctx.set_stroke(0xffed2f31);
        ctx.set_line_width(7);
        ctx.begin_path();
        ctx.move_to(2 * w * ent.health_ratio, 0);
        ctx.line_to(2 * w * ent.healthbar_lag, 0);
        ctx.stroke();
    }
    ctx.set_stroke(0xff75dd34);
    ctx.set_line_width(7);
    ctx.begin_path();
    ctx.move_to(0, 0);
    ctx.line_to(2 * w * ent.health_ratio, 0);
    ctx.stroke();
}