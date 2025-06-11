#include <Client/Assets/Assets.hh>

#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <iostream>
#include <cmath>

void draw_static_petal_single(PetalID::T id, Renderer &ctx) {
    float r = PETAL_DATA[id].radius;
    switch(id) {
        case PetalID::kNone:
            break;
        case PetalID::kDandelion:
            ctx.set_stroke(0xff222222);
            ctx.round_line_cap();
            ctx.set_line_width(7);
            ctx.begin_path();
            ctx.move_to(0,0);
            ctx.line_to(-1.6 * r, 0);
            ctx.stroke();
        case PetalID::kUniqueBasic:
        case PetalID::kBasic:
        case PetalID::kLight:
        case PetalID::kTwin:
        case PetalID::kTriplet:
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kHeavy:
        case PetalID::kShield:
            ctx.set_fill(0xffaaaaaa);
            ctx.set_stroke(0xff888888);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kStinger: {
        case PetalID::kTringer:
            ctx.set_fill(0xff333333);
            ctx.set_stroke(0xff292929);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(r,0);
            ctx.line_to(-r*0.5,r*0.866);
            ctx.line_to(-r*0.5,-r*0.866);
            ctx.line_to(r,0);
            ctx.fill();
            ctx.stroke();
            break;
        }
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
        case PetalID::kRose:
        case PetalID::kDahlia:
            ctx.set_fill(0xffff94c9);
            ctx.set_stroke(0xffcf78a3);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kAntEgg:
            ctx.set_stroke(0xffcfc295);
            ctx.set_fill(0xfffff0b8);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kBeetleEgg:
            ctx.begin_path();
            ctx.ellipse(0,0,r * 0.85, r * 1.15);
            ctx.set_fill(0xfffff0b8);
            ctx.fill();
            ctx.set_stroke(0xffcfc295);
            ctx.set_line_width(3);
            ctx.stroke();
            break;
        case PetalID::kMissile:
            ctx.scale(r / 10);
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
        case PetalID::kIris:
            ctx.set_fill(0xffce76db);
            ctx.set_stroke(0xffa760b1);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kPollen:
            ctx.set_fill(0xffffe763);
            ctx.set_stroke(0xffcfbb50);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kBubble:
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.set_stroke(0xb2ffffff);
            ctx.set_line_width(3);
            ctx.stroke();
            ctx.begin_path();
            ctx.arc(0,0,r-1.5);
            ctx.set_fill(0x59ffffff);
            ctx.fill();
            ctx.begin_path();
            ctx.arc(r/3,-r/3,r/4);
            ctx.set_fill(0x59ffffff);
            ctx.fill();
            break;
        case PetalID::kFaster:
            ctx.set_fill(0xfffeffc9);
            ctx.set_stroke(0xffcecfa3);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kThirdEye:
            ctx.scale(0.5);
            ctx.set_fill(0xff111111);
            ctx.begin_path();
            ctx.move_to(0,-10);
            ctx.qcurve_to(8,0,0,10);
            ctx.qcurve_to(-8,0,0,-10);
            ctx.fill();
            ctx.set_fill(0xffeeeeee);
            ctx.begin_path();
            ctx.arc(0, 0, 5);
            ctx.fill();
            ctx.set_stroke(0xff111111);
            ctx.set_line_width(1.5);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(0,-10);
            ctx.qcurve_to(8,0,0,10);
            ctx.qcurve_to(-8,0,0,-10);
            ctx.stroke();
            break;
        case PetalID::kWeb:
        case PetalID::kTriweb:
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(11.00, 0.00);
            ctx.qcurve_to(4.32, 3.14, 3.40, 10.46);
            ctx.qcurve_to(-1.65, 5.08, -8.90, 6.47);
            ctx.qcurve_to(-5.34, -0.00, -8.90, -6.47);
            ctx.qcurve_to(-1.65, -5.08, 3.40, -10.46);
            ctx.qcurve_to(4.32, -3.14, 11.00, 0.00);
            ctx.fill();
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
        case PetalID::kRock: {
            ctx.set_fill(0xff777777);
            ctx.set_stroke(Renderer::HSV(0xff777777, 0.8));
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(12.138091087341309,0);
            ctx.line_to(3.8414306640625,12.377452850341797);
            ctx.line_to(-11.311542510986328,7.916932582855225);
            ctx.line_to(-11.461170196533203,-7.836822032928467);
            ctx.line_to(4.538298606872559,-13.891617774963379);
            ctx.line_to(12.138091087341309,0);
            ctx.close_path();
            ctx.fill();
            ctx.stroke();
            break;
        }
        case PetalID::kAntennae: {
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.set_stroke(0xff333333);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(5, 12.5);
            ctx.qcurve_to(10, -2.5, 15, -12.5);
            ctx.qcurve_to(5, -2.5, 5, 12.5);
            ctx.move_to(-5, 12.5);
            ctx.qcurve_to(-10, -2.5, -15, -12.5);
            ctx.qcurve_to(-5, -2.5, -5, 12.5);
            ctx.fill();
            ctx.stroke();
            break;
        }
        case PetalID::kObserver: {
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.set_stroke(0xff333333);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(5, 12.5);
            ctx.qcurve_to(10, -2.5, 15, -12.5);
            ctx.qcurve_to(5, -2.5, 5, 12.5);
            ctx.move_to(-5, 12.5);
            ctx.qcurve_to(-10, -2.5, -15, -12.5);
            ctx.qcurve_to(-5, -2.5, -5, 12.5);
            ctx.fill();
            ctx.stroke();
            ctx.set_fill(0xffd01c1d);
            ctx.begin_path();
            ctx.arc(15, -12.5, 2.5);
            ctx.close_path();
            ctx.arc(-15, -12.5, 2.5);
            ctx.close_path();
            ctx.fill();
            break;
        }
        case PetalID::kBlueIris:
            ctx.set_fill(0xff39e9f1);
            ctx.set_stroke(0xff2dbac0);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,r);
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
        case PetalID::kPoisonPeas:
            ctx.set_fill(0xffce76db);
            ctx.set_stroke(0xffa760b1);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.arc(0,0,PETAL_DATA[id].radius);
            ctx.fill();
            ctx.stroke();
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
        case PetalID::kSand:
            ctx.set_fill(0xffe0c85c);
            ctx.set_stroke(0xffb5a24b);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(7,0);
            ctx.line_to(3.499999761581421,6.062178134918213);
            ctx.line_to(-3.500000476837158,6.062177658081055);
            ctx.line_to(-7,-6.119594218034763e-7);
            ctx.line_to(-3.4999992847442627,-6.062178134918213);
            ctx.line_to(3.4999992847442627,-6.062178134918213);
            ctx.close_path();
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kStick:
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.set_line_width(7);
            ctx.set_stroke(0xff654a19);
            ctx.begin_path();
            ctx.move_to(0,10);
            ctx.line_to(0,0);
            ctx.line_to(4,-7);
            ctx.move_to(0,0);
            ctx.line_to(-6,-10);
            ctx.stroke();
            ctx.set_line_width(3);
            ctx.set_stroke(0xff7d5b1f);
            ctx.stroke();
            break;
        case PetalID::kPincer:
            ctx.set_fill(0xff333333);
            ctx.set_stroke(0xff292929);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(10,5);
            ctx.qcurve_to(4,-14,-10,5);
            ctx.qcurve_to(4,0,10,5);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kAzalea: {
            ctx.set_fill(0xffff94c9);
            ctx.set_stroke(0xffcf78a3);
            ctx.set_line_width(3);
            ctx.begin_path();
            float l = 0.75 * r;
            float h = 1.5 * r;
            uint32_t s = 5;
            ctx.move_to(l, 0);
            for (uint32_t i = 1; i <= s; ++i) {
                float angle = i * 2 * M_PI / s;
                float angle2 = angle - M_PI / s;
                ctx.qcurve_to(h * cosf(angle2), h * sinf(angle2), l * cosf(angle),l * sinf(angle));
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
        case PetalID::kSalt:
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(3);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(10.404077529907227,0);
            ctx.line_to(6.643442630767822,8.721502304077148);
            ctx.line_to(-2.6667866706848145,11.25547981262207);
            ctx.line_to(-10.940428733825684,4.95847225189209);
            ctx.line_to(-11.341578483581543,-5.432167053222656);
            ctx.line_to(-2.4972469806671143,-11.472168922424316);
            ctx.line_to(7.798409461975098,-9.584606170654297);
            ctx.line_to(10.404077529907227,0);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kSquare:
            ctx.set_fill(0xffffe869);
            ctx.set_stroke(0xffcfbc55);
            ctx.set_line_width(0.15*r);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.rect(-r * 0.707, -r * 0.707, r * 1.414, r * 1.414);
            ctx.fill();
            ctx.stroke();
            break;
        case PetalID::kMoon: {
            ctx.set_fill(0xff878787);
            ctx.set_stroke(0xff6d6d6d);
            ctx.set_line_width(5);
            ctx.begin_path();
            ctx.arc(0,0,r);
            ctx.stroke();
            ctx.fill();
            ctx.clip();
            SeedGenerator gen(id * 274633 + 284562);
            uint32_t ct = 10;
            ctx.set_fill(0xff999999);
            ctx.set_stroke(0xff7c7c7c);
            ctx.set_line_width(3);
            ctx.begin_path();
            for (uint32_t i = 0; i < ct; ++i) {
                float _x = gen.binext() * (r + 10);
                float _y = gen.binext() * (r + 10);
                float _r = gen.binext() * 10 + 10;
                ctx.move_to(_x,_y);
                ctx.arc(_x,_y,_r);
            }
            ctx.stroke();
            ctx.fill();
            break;
        }
        /*
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
        */
        default:
            std::cout << (int) id << '\n';
            assert(id < PetalID::kNumPetals);
            assert(!"didn't cover petal render");
            break;
    }
}

void draw_static_petal(PetalID::T id, Renderer &ctx) {
    struct PetalData const &data = PETAL_DATA[id];
    uint32_t count = data.count;
    if (count == 0) count = 1;
    for (uint32_t i = 0; i < count; ++i) {
        RenderContext context(&ctx);
        float rad = 10;
        if (data.attributes.clump_radius != 0)
            rad = data.attributes.clump_radius;
        ctx.rotate(i * 2 * M_PI / data.count);
        if (data.count > 1) ctx.translate(rad, 0);
        ctx.rotate(data.attributes.icon_angle);
        draw_static_petal_single(id, ctx);
    }
}

void draw_loadout_background(Renderer &ctx, uint8_t id, float reload) {
    RenderContext c(&ctx);
    ctx.set_fill(Renderer::HSV(RARITY_COLORS[PETAL_DATA[id].rarity], 0.8));
    ctx.round_line_join();
    ctx.round_line_cap();
    ctx.begin_path();
    ctx.round_rect(-30, -30, 60, 60, 3);
    ctx.fill();
    ctx.set_fill(RARITY_COLORS[PETAL_DATA[id].rarity]);
    ctx.begin_path();
    ctx.rect(-25, -25, 50, 50);
    ctx.fill();
    ctx.clip();
    if (reload < 1) {
        float rld =  1 - (float) reload;
        {
            rld = rld * rld * rld * (rld * (6.0f * rld - 15.0f) + 10.0f);
            RenderContext context(&ctx);
            ctx.set_fill(0x40000000);
            ctx.begin_path();
            ctx.move_to(0,0);
            ctx.partial_arc(0, 0, 90, -M_PI / 2 - rld * M_PI * 10, -M_PI / 2 - rld * M_PI * 8, 0);
            ctx.fill();
        }
    }
    ctx.translate(0, -5);
    {
        RenderContext r(&ctx);
        ctx.scale(0.833);
        if (PETAL_DATA[id].radius > 20) ctx.scale(20 / PETAL_DATA[id].radius);
        draw_static_petal(id, ctx);
    }
    float text_width = 12 * Renderer::get_ascii_text_size(PETAL_DATA[id].name);
    if (text_width < 50) text_width = 12;
    else text_width = 12 * 50 / text_width;
    ctx.translate(0, 20);
    ctx.draw_text(PETAL_DATA[id].name, { .size = text_width });
}