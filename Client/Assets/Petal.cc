#include <Client/Assets/Assets.hh>

#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <cmath>

void draw_static_petal_single(uint8_t id, Renderer &ctx) {
    switch(id) {
        case PetalID::kNone:
            break;
        case PetalID::kBasic:
        case PetalID::kLight:
        //case PetalID::kTwin:
        //case PetalID::kTriplet:
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius * 0.5);
            ctx.fill();
            ctx.stroke();
            break;
        /*
        case PetalID::kStinger:
        case PetalID::kTringer:
            ctx.set_fill(0xff333333);
            ctx.set_stroke(0xff292929);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(PETAL_DATA[id].radius,0);
            ctx.line_to(-PETAL_DATA[id].radius*0.5,PETAL_DATA[id].radius*0.866);
            ctx.line_to(-PETAL_DATA[id].radius*0.5,-PETAL_DATA[id].radius*0.866);
            ctx.line_to(PETAL_DATA[id].radius,0);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kLeaf:
            ctx.set_fill(0xff39b54a);
            ctx.set_stroke(0xff2e933c);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(-20, 0);
            ctx.line_to(-15, 0);
            ctx.bcurve_to(-10,-12,5,-12,15,0);
            ctx.bcurve_to(5,12,-10,12,-15,0);
            ctx.fill();
            ctx.stroke();
            ctx.begin_path();
            ctx.move_to(-9,0);
            ctx.qcurve_to(0,-1.5,7.5,0);
            ctx.stroke();
            break;
        case PetalID::kWing:
            ctx.begin_path();
            ctx.partial_arc(0,0,15,-1.5707963267948966,1.5707963267948966,0);
            ctx.qcurve_to(10,0,0,-15);
            ctx.set_fill(0xffffffff);
            ctx.fill();
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.stroke();
            break;
        case PetalID::kEgg:
            ctx.set_fill(0xffcfc295);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.set_fill(0xfffff0b8);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius * 0.8);
            ctx.fill();
            break;
        case PetalID::kBeetleEgg:
            ctx.begin_path();
            ctx.ellipse(0,0,PETAL_DATA[id].radius * 0.85,PETAL_DATA[id].radius * 1.15);
            ctx.set_fill(0xfffff0b8);
            ctx.fill();
            ctx.set_stroke(0xffcfc295);
            ctx.set_line_width(3);
            ctx.stroke();
            break;
        case PetalID::kIris:
        case PetalID::kGrapes:
            ctx.set_fill(0xffce76db);
            ctx.set_stroke(0xffa760b1);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kMissile:
            ctx.set_fill(0xff222222);
            ctx.set_stroke(0xff222222);
            ctx.set_line_width(5.0);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(11.0, 0.0);
            ctx.line_to(-11.0, -6.0);
            ctx.line_to(-11.0, 6.0);
            ctx.line_to(11.0, 0.0);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kBubble:
            ctx.begin_path();
            ctx.arc(0,0,12);
            ctx.set_stroke(0xb2ffffff);
            ctx.set_line_width(3);
            ctx.stroke();
            ctx.begin_path();
            ctx.arc(0,0,10.5);
            ctx.set_fill(0x59ffffff);
            ctx.fill();
            ctx.begin_path();
            ctx.arc(4,-4,3);
            ctx.set_fill(0x59ffffff);
            ctx.fill();
            break;
        case PetalID::kPeas:
            ctx.set_fill(0xff8ac255);
            ctx.set_stroke(0xff709d45);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kRock: {
            SeedGenerator gen(PETAL_DATA[id].radius * 743 + 294);
            ctx.set_fill(0xff777777);
            ctx.set_stroke(Renderer::HSV(0xff777777, 0.8));
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(PETAL_DATA[id].radius + (gen.next() - 0.5) * 2,(gen.next() - 0.5) * 2);
            for (uint32_t i = 1; i < 5; ++i) {
                float angle = 2 * M_PI * i / 5.0f;
                ctx.line_to(cosf(angle) * PETAL_DATA[id].radius + (gen.next() - 0.5) * 2, sinf(angle) * PETAL_DATA[id].radius + (gen.next() - 0.5) * 2);
            }
            ctx.close_path();
            ctx.fill();
            ctx.stroke();
            break;
        }
        case PetalID::kHeavy:
            ctx.set_fill(0xffaaaaaa);
            ctx.set_stroke(0xff888888);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kHeaviest:
            ctx.begin_path();
            ctx.arc(0,0,16);
            ctx.set_fill(0xff333333);
            ctx.fill();
            ctx.set_stroke(0xff292929);
            ctx.set_line_width(3);
            ctx.stroke();
            ctx.begin_path();
            ctx.arc(6,-6,4.6);
            ctx.set_fill(0xffcccccc);
            ctx.fill();
            break;
        case PetalID::kRose:
        case PetalID::kAzalea:
            ctx.set_fill(0xffff94c9);
            ctx.set_stroke(0xffcf78a3);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kCactus:
        case PetalID::kTricac:
            ctx.set_fill(0xff38c75f);
            ctx.set_stroke(Renderer::HSV(0xff38c75f, 0.8));
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(15,0);
            for (uint32_t i = 0; i < 8; ++i) {
                float base_angle = M_PI * 2 * i / 8;
                ctx.qcurve_to(15*0.8*cosf(base_angle+M_PI/8),15*0.8*sinf(base_angle+M_PI/8),15*cosf(base_angle+2*M_PI/8),15*sinf(base_angle+2*M_PI/8));
            }
            ctx.fill();
            ctx.stroke();
            ctx.set_fill(0xff74d68f);
            ctx.begin_path();
            ctx.arc(0,0,8);
            ctx.fill();
            break;
        case PetalID::kEAzalea: {
            ctx.set_fill(0xffff94c9);
            ctx.set_stroke(0xffcf78a3);
            ctx.set_line_width(3);
            ctx.begin_path();
            float r = PETAL_DATA[id].radius;
            float base = M_PI / 10;
            ctx.move_to(r * cosf(base), r * sinf(base));
            for (uint32_t i = 0; i < 5; ++i) {
                ctx.qcurve_to(1.2 * r * cosf((4 * i + 2) * base), 1.2 * r * sinf((4 * i + 2) * base), r * cosf((4 * i + 3) * base), r * sinf((4 * i + 3) * base));
                ctx.qcurve_to(0.8 * r * cosf((4 * i + 4) * base), 0.8 * r * sinf((4 * i + 4) * base), r * cosf((4 * i + 5) * base), r * sinf((4 * i + 5) * base));
            }
            ctx.fill();
            ctx.stroke();
            break;
        }
        case PetalID::kPoisonCactus:
            ctx.set_fill(0xffce76db);
            ctx.set_stroke(0xffa760b1);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(15,0);
            for (uint32_t i = 0; i < 8; ++i) {
                float base_angle = M_PI * 2 * i / 8;
                ctx.qcurve_to(15*0.8*cosf(base_angle+M_PI/8),15*0.8*sinf(base_angle+M_PI/8),15*cosf(base_angle+2*M_PI/8),15*sinf(base_angle+2*M_PI/8));
            }
            ctx.fill();
            ctx.stroke();
            ctx.set_fill(0xffcea0db);
            ctx.begin_path();
            ctx.arc(0,0,8);
            ctx.fill();
            break;
        */
        default:
            assert(!"didn't cover petal render");
            break;
    }
}

void draw_static_petal(uint8_t id, Renderer &ctx) {
    for (uint32_t i = 0; i < PETAL_DATA[id].count; ++i) {
        RenderContext context(&ctx);
        //float rad = PETAL_DATA[id].extras.clump_radius;
        //rad += (rad == 0) * 10;
        //ctx.rotate(i * 2 * M_PI / PETAL_DATA[id].count);
        //if (PETAL_DATA[id].count > 1) ctx.translate(rad, 0);
        //if (id == PetalID::kLeaf) ctx.rotate(-1);
        //if (id == PetalID::kWing || id == PetalID::kMissile) ctx.rotate(1);
        draw_static_petal_single(id, ctx);
    }
}