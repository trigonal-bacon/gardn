#include <Client/Render/RenderEntity.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Render/Renderer.hh>

#include <Client/Game.hh>

#include <Shared/Entity.hh>

#include <iostream>

void render_flower(Renderer &ctx, Entity const &ent) {
    if (ent.has_component(kMob)) return;
    ctx.set_global_alpha(1 - ent.deletion_animation);
    ctx.scale(1 + 0.5 * ent.deletion_animation);
    /*
    {
        //nametag
        std::cout << ent.name << '\n';
        RenderContext context(&ctx);
        ctx.set_fill(0xffffffff);
        ctx.set_stroke(0xff222222);
        ctx.set_text_size(20);
        ctx.set_line_width(20 * 0.12);
        ctx.center_text_align();
        ctx.center_text_baseline();
        ctx.translate(0,-ent.radius - 25);
        char const *ptr = ent.name.c_str();
        ctx.stroke_text(ptr);
        ctx.fill_text(ptr);
    }
    */
    if (ent.damage_flash > 0.75)
        ctx.add_color_filter(0xffffffff, ent.damage_flash * 0.8);
    else if (ent.damage_flash > 0.01)
        ctx.add_color_filter(0xffac0000, ent.damage_flash * 1.5);
    draw_static_flower(ctx, {
        .radius = ent.radius,
        .eye_x = ent.eye_x,
        .eye_y = ent.eye_y,
        .mouth = ent.mouth,
        .cutter_angle = (float) (Game::timestamp / 200),
        .face_flags = ent.face_flags,
        .flags = 0
    });
}