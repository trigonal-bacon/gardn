#include <Client/Assets/Assets.hh>

void draw_static_flower(Renderer &ctx, FlowerRenderAttributes attributes) {
    if (BIT_AT(attributes.face_flags, 7)) {
        RenderContext context(&ctx);
        ctx.scale(attributes.radius / 25);
        ctx.rotate(attributes.cutter_angle);
        draw_static_petal_single(PetalID::kCutter, ctx);
    }
    uint32_t base_color = 0xffffe763;
    if (BIT_AT(attributes.face_flags, 2)) base_color = 0xffce76db;
    else if (BIT_AT(attributes.face_flags, 3)) base_color = Renderer::MIX(base_color, 0xffcfcfcf, 0.5);
    else if (BIT_AT(attributes.flags, 0)) base_color = 0xff999999;
    ctx.set_stroke(Renderer::HSV(base_color, 0.8));
    ctx.set_fill(base_color);
    ctx.set_line_width(3);
    ctx.begin_path();
    ctx.arc(0, 0, attributes.radius);
    ctx.fill();
    ctx.stroke();
    ctx.scale(attributes.radius / 25);
    {
        RenderContext context(&ctx);
        ctx.set_fill(0xff222222);
        ctx.begin_path();
        if (BIT_AT(attributes.flags, 0)) {
            ctx.rect(-7-3.25, -5-6.5, 6.5, 13);
            ctx.rect(7-3.25, -5-6.5, 6.5, 13);
            ctx.fill();
            ctx.begin_path();
            ctx.rect(-7-3, -5-6, 6, 12);
            ctx.rect(7-3, -5-6, 6, 12);
            ctx.clip();
            ctx.set_fill(0xffffffff);
            ctx.begin_path();
            ctx.rect(-7 + attributes.eye_x-3, -5 + attributes.eye_y-3, 6, 6);
            ctx.rect(7 + attributes.eye_x-3, -5 + attributes.eye_y-3, 6, 6);
            ctx.fill();
        } else {
            ctx.ellipse(-7, -5, 3.25, 6.5);
            ctx.ellipse(7, -5, 3.25, 6.5);
            ctx.fill();
            ctx.begin_path();
            ctx.ellipse(-7, -5, 3, 6);
            ctx.ellipse(7, -5, 3, 6);
            ctx.clip();
            ctx.set_fill(0xffffffff);
            ctx.begin_path();
            ctx.arc(-7 + attributes.eye_x, -5 + attributes.eye_y, 3);
            ctx.arc(7 + attributes.eye_x, -5 + attributes.eye_y, 3);
            ctx.fill();
        }
    }
    ctx.set_stroke(0xff222222);
    ctx.set_line_width(1.5);
    ctx.round_line_cap();
    ctx.begin_path();
    ctx.move_to(-6, 10);
    ctx.qcurve_to(0, attributes.mouth, 6, 10);
    ctx.stroke();
    if (attributes.mouth <= 8 && BIT_AT(attributes.face_flags, 0))
    {
        RenderContext context(&ctx);
        ctx.translate(0, -attributes.mouth - 8);
        ctx.set_fill(base_color);
        ctx.begin_path();
        ctx.move_to(-12, 0);
        ctx.line_to(12, 0);
        ctx.line_to(0, 6);
        ctx.line_to(-12, 0);
        ctx.fill();
    }
    if (BIT_AT(attributes.face_flags, 4))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -14);
        ctx.scale(0.5);
        draw_static_petal(PetalID::kThirdEye, ctx);
    }
    if (BIT_AT(attributes.face_flags, 6))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        draw_static_petal(PetalID::kObserver, ctx);
    } else if (BIT_AT(attributes.face_flags, 5))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        draw_static_petal(PetalID::kAntennae, ctx);
    }
}
