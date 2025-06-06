#include <Client/Render/RenderEntity.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Render/Renderer.hh>

#include <Shared/Entity.hh>

#include <iostream>

void render_flower(Renderer &ctx, Entity const &ent) {
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
    uint32_t base_color = 0xffffe763;
    if (BIT_AT(ent.face_flags, 2)) base_color = 0xffce76db;
    else if (BIT_AT(ent.face_flags, 3)) base_color = Renderer::MIX(base_color, 0xffcfcfcf, 0.5);
    ctx.set_stroke(Renderer::HSV(base_color, 0.8));
    ctx.set_fill(base_color);
    ctx.set_line_width(3);
    ctx.begin_path();
    ctx.arc(0, 0, ent.radius);
    ctx.fill();
    ctx.stroke();
    ctx.scale(ent.radius / 25);
    {
        RenderContext context(&ctx);
        ctx.set_fill(0xff222222);
        ctx.begin_path();
        ctx.ellipse(-7, -5, 3.25, 6.5);
        ctx.ellipse(7, -5, 3.25, 6.5);
        ctx.fill();
        ctx.begin_path();
        ctx.ellipse(-7, -5, 3, 6);
        ctx.ellipse(7, -5, 3, 6);
        ctx.clip();
        ctx.set_fill(0xffffffff);
        ctx.begin_path();
        ctx.arc(-7 + ent.eye_x, -5 + ent.eye_y, 3);
        ctx.arc(7 + ent.eye_x, -5 + ent.eye_y, 3);
        ctx.fill();
    }
    ctx.set_stroke(0xff222222);
    ctx.set_line_width(1.5);
    ctx.round_line_cap();
    ctx.begin_path();
    ctx.move_to(-6, 10);
    ctx.qcurve_to(0, ent.mouth, 6, 10);
    ctx.stroke();
    if (ent.mouth <= 8 && BIT_AT(ent.face_flags, 0))
    {
        RenderContext context(&ctx);
        ctx.translate(0, -ent.mouth - 8);
        ctx.set_fill(base_color);
        ctx.begin_path();
        ctx.move_to(-12, 0);
        ctx.line_to(12, 0);
        ctx.line_to(0, 6);
        ctx.line_to(-12, 0);
        ctx.fill();
    }
    if (BIT_AT(ent.face_flags, 4))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -14);
        ctx.scale(0.5);
        //rr_renderer_scale(renderer, 0.625);
        draw_static_petal(PetalID::kThirdEye, ctx);
        //ctx.scale(1.6);
        //rr_renderer_translate(renderer, 0, 15);
    }
    if (BIT_AT(ent.face_flags, 6))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        //ctx.scale(0.5);
        //rr_renderer_scale(renderer, 0.625);
        draw_static_petal(PetalID::kObserver, ctx);
        //ctx.scale(1.6);
        //rr_renderer_translate(renderer, 0, 15);
    } else if (BIT_AT(ent.face_flags, 5))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        //ctx.scale(0.5);
        //rr_renderer_scale(renderer, 0.625);
        draw_static_petal(PetalID::kAntennae, ctx);
        //ctx.scale(1.6);
        //rr_renderer_translate(renderer, 0, 15);
    }
}