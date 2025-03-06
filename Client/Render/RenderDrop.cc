#include <Client/Render/RenderEntity.hh>

#include <Client/Render/Renderer.hh>

#include <Client/Assets/Assets.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void render_drop(Renderer &ctx, Entity &ent) {
    float animation_value = sinf(ent.animation);
    ctx.set_global_alpha(1 - ent.deletion_tick * 0.2);
    ctx.scale(1 - 0.2 * ent.deletion_tick);
    ctx.scale(1 + animation_value * 0.03);
    ctx.scale(ent.radius / 30);
    uint8_t rarity = PETAL_DATA[ent.drop_id].rarity;
    ctx.set_fill(0x80000000);
    ctx.begin_path();
    ctx.rect(-40, -40, 80, 80);
    ctx.fill();
    draw_loadout_background(ctx, RARITY_COLORS[rarity]);
    ctx.translate(0, -5);
    {
        RenderContext r(&ctx);
        if (PETAL_DATA[ent.drop_id].radius > 30) ctx.scale(30 / PETAL_DATA[ent.drop_id].radius);
        draw_static_petal(ent.drop_id, ctx);
    }
    ctx.set_text_size(14);
    ctx.set_line_width(14 * 0.12);
    ctx.set_fill(0xffffffff);
    ctx.set_stroke(0xff222222);
    ctx.translate(0, 20);
    ctx.center_text_align();
    ctx.center_text_baseline();
    ctx.stroke_text(PETAL_DATA[ent.drop_id].name);
    ctx.fill_text(PETAL_DATA[ent.drop_id].name);
}