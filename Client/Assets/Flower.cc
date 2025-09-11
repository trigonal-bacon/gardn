#include <Client/Assets/Assets.hh>

#include <Client/StaticData.hh>

#include <Helpers/Bits.hh>

void draw_static_flower(Renderer &ctx, FlowerRenderAttributes attributes) {
    if (BitMath::at(attributes.equip_flags, EquipmentFlags::kCutter)) {
        RenderContext context(&ctx);
        ctx.scale(attributes.radius / 25);
        ctx.rotate(attributes.cutter_angle);
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
    ctx.arc(0, 0, attributes.radius);
    ctx.fill();
    ctx.stroke();
    ctx.scale(attributes.radius / 25);
    {
        RenderContext context(&ctx);
        ctx.set_fill(0xff222222);
        ctx.set_stroke(0xff222222);
        ctx.set_line_width(1);
        ctx.begin_path();
        if (BitMath::at(attributes.face_flags, FaceFlags::kDeadEyes)) {
            float const len = 4;
            ctx.set_stroke(0xff222222);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(-7-len, -5-len);
            ctx.line_to(-7+len,-5+len);
            ctx.move_to(-7+len,-5-len);
            ctx.line_to(-7-len,-5+len);
            ctx.move_to(7-len, -5-len);
            ctx.line_to(7+len,-5+len);
            ctx.move_to(7+len,-5-len);
            ctx.line_to(7-len,-5+len);
            ctx.stroke();
        } else if (BitMath::at(attributes.face_flags, FaceFlags::kSquareEyes)) {
            ctx.rect(-7-3, -5-6, 6, 12);
            ctx.move_to(7, -5);
            ctx.rect(7-3, -5-6, 6, 12);
            ctx.fill();
            ctx.stroke();
            ctx.clip();
            ctx.set_fill(0xffffffff);
            ctx.begin_path();
            ctx.rect(-7 + attributes.eye_x-3, -5 + attributes.eye_y-3, 6, 6);
            ctx.rect(7 + attributes.eye_x-3, -5 + attributes.eye_y-3, 6, 6);
            ctx.fill();
        } else {
            ctx.ellipse(-7, -5, 3, 6);
            ctx.move_to(7, -5);
            ctx.ellipse(7, -5, 3, 6);
            ctx.fill();
            ctx.stroke();
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
    if (!BitMath::at(attributes.face_flags, FaceFlags::kDeadEyes) && attributes.mouth <= 8 && BitMath::at(attributes.face_flags, FaceFlags::kAttacking))
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
