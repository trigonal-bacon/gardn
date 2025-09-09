#include <Client/Assets/Assets.hh>

#include <Client/StaticData.hh>

#include <Helpers/Bits.hh>

#include <functional>

static void draw_eyes(Renderer &ctx, FlowerRenderAttributes attributes, std::function<void(void)> draw) {
    {
        RenderContext context(&ctx);
        if (!BitMath::at(attributes.face_flags, FaceFlags::kDeveloper))
            ctx.translate(-7, -5);
        else {
            ctx.translate(-10, -10);
            ctx.scale(-1);
        }
        draw();
    }
    {
        RenderContext context(&ctx);
        if (!BitMath::at(attributes.face_flags, FaceFlags::kDeveloper))
            ctx.translate(7, -5);
        else
            ctx.translate(10, 8);
        draw();
    }
}

void draw_static_flower(Renderer &ctx, FlowerRenderAttributes attributes) {
    ctx.scale(attributes.radius / 25);
    if (BitMath::at(attributes.equip_flags, EquipmentFlags::kCutter)) {
        RenderContext context(&ctx);
        ctx.rotate(attributes.cutter_angle);
        ctx.set_fill(0xff111111);
        ctx.begin_path();
        ctx.arc(0, 0, 27.5);
        ctx.fill();
        draw_static_petal_single(PetalID::kCutter, ctx);
    }
    uint32_t base_color = FLOWER_COLORS[attributes.color];
    if (BitMath::at(attributes.face_flags, FaceFlags::kPoisoned)) 
        base_color = 0xffce76db;
    else if (BitMath::at(attributes.face_flags, FaceFlags::kDandelioned)) 
        base_color = Renderer::MIX(base_color, 0xffffffff, 0.4);
    ctx.set_stroke(Renderer::HSV(base_color, 0.8));
    ctx.set_fill(base_color);
    ctx.set_line_width(3);
    ctx.begin_path();
    if (!BitMath::at(attributes.face_flags, FaceFlags::kDeveloper))
        ctx.arc(0, 0, 25);
    else {
        ctx.move_to(25, 0);
        ctx.qcurve_to(15, 35, 0, 25);
        ctx.qcurve_to(-25, 15, -25, 0);
        ctx.qcurve_to(-25, -35, 0, -25);
        ctx.qcurve_to(18, -25, 25, 0);
    }
    ctx.fill();
    ctx.stroke();
    {
        RenderContext context(&ctx);
        ctx.set_fill(0xff222222);
        ctx.begin_path();
        if (BitMath::at(attributes.face_flags, FaceFlags::kDeadEyes)) {
            float const len = 4;
            ctx.set_stroke(0xff222222);
            ctx.set_line_width(3);
            draw_eyes(ctx, attributes, [&](){
                ctx.move_to(-len, -len);
                ctx.line_to(len,len);
                ctx.move_to(len,-len);
                ctx.line_to(-len,len);
            });
            ctx.stroke();
        } else if (BitMath::at(attributes.face_flags, FaceFlags::kSquareEyes)) {
            draw_eyes(ctx, attributes, [&](){
                ctx.rect(-3.25, -6.5, 6.5, 13);
            });
            ctx.fill();
            ctx.begin_path();
            draw_eyes(ctx, attributes, [&](){
                ctx.rect(-3, -6, 6, 12);
            });
            ctx.clip();
            ctx.set_fill(0xffffffff);
            ctx.begin_path();
            draw_eyes(ctx, attributes, [&](){
                ctx.rect(attributes.eye_x-3, attributes.eye_y-3, 6, 6);
            });
            ctx.fill();
        } else {
            draw_eyes(ctx, attributes, [&](){
                ctx.ellipse(0, 0, 3.25, 6.5);
            });
            ctx.fill();
            ctx.begin_path();
            draw_eyes(ctx, attributes, [&](){
                ctx.ellipse(0, 0, 3, 6);
            });
            ctx.clip();
            ctx.set_fill(0xffffffff);
            ctx.begin_path();
            draw_eyes(ctx, attributes, [&](){
                ctx.arc(attributes.eye_x, attributes.eye_y, 3);
            });
            ctx.fill();
        }
    }
    if (!BitMath::at(attributes.face_flags, FaceFlags::kDeveloper))
        ctx.translate(0, 10);
    else {
        ctx.translate(-8, 7);
        ctx.rotate(M_PI / 5);
    }
    ctx.set_stroke(0xff222222);
    ctx.set_line_width(1.5);
    ctx.round_line_cap();
    ctx.begin_path();
    ctx.move_to(-6, 0);
    ctx.qcurve_to(0, attributes.mouth - 10, 6, 0);
    ctx.stroke();
    ctx.translate(0, -10);
    if (!BitMath::at(attributes.face_flags, FaceFlags::kDeadEyes) && !BitMath::at(attributes.face_flags, FaceFlags::kDeveloper) &&
        attributes.mouth <= 8 && BitMath::at(attributes.face_flags, FaceFlags::kAttacking))
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
    if (BitMath::at(attributes.equip_flags, EquipmentFlags::kThirdEye))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -14);
        ctx.scale(0.5);
        if (BitMath::at(attributes.face_flags, FaceFlags::kDeadEyes)) {
            float const len = 4;
            ctx.set_stroke(0xff222222);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(-len,-len);
            ctx.line_to(len,len);
            ctx.move_to(len,-len);
            ctx.line_to(-len,len);
            ctx.stroke();
        }
        else
            draw_static_petal(PetalID::kThirdEye, ctx);
    }
    if (BitMath::at(attributes.equip_flags, EquipmentFlags::kObserver))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        draw_static_petal(PetalID::kObserver, ctx);
    } else if (BitMath::at(attributes.equip_flags, EquipmentFlags::kAntennae))
    {
        RenderContext g(&ctx);
        ctx.translate(0, -35);
        draw_static_petal(PetalID::kAntennae, ctx);
    }
}
