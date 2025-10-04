#include <Client/Assets/Assets.hh>

#include <Client/StaticData.hh>

#include <Helpers/Bits.hh>
#include <Helpers/Math.hh>

#include <cmath>

#define SET_BASE_COLOR(set_color) { if (!BitMath::at(flags, 0)) base_color = set_color; else { base_color = FLOWER_COLORS[attr.color]; } }


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
        case PetalID::kSoil: {
    uint32_t base_color = 0xff695118; 
    SeedGenerator gen(std::floor(r) * 1951264 + 295726);
    ctx.set_fill(base_color);
    ctx.set_stroke(Renderer::HSV(base_color, 0.8));
    ctx.set_line_width(5);
    ctx.round_line_cap();
    ctx.round_line_join();
    ctx.begin_path();
    float deflection = r * 0.1;
    ctx.move_to(r + gen.binext() * deflection, gen.binext() * deflection);
    uint32_t sides = 7 + r / 10;
    for (uint32_t i = 1; i < sides; ++i) {
        float angle = 2 * M_PI * i / sides;
        ctx.line_to(cosf(angle) * r + gen.binext() * deflection,
                    sinf(angle) * r + gen.binext() * deflection);
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
        case PetalID::kBasil: 
        ctx.scale(r / 17.5);
        ctx.translate(-51, -50);
            ctx.set_fill(0xff288034);
ctx.begin_path();
ctx.move_to(54.63, 46.61);
ctx.qcurve_to(45.99, 36.79, 53.57, 29.45);
ctx.qcurve_to(57.05, 26.08, 66.86, 22.68);
ctx.qcurve_to(67.70, 22.39, 68.09, 22.25);
ctx.qcurve_to(68.26, 22.19, 68.43, 22.15);
ctx.qcurve_to(68.60, 22.11, 68.77, 22.08);
ctx.qcurve_to(69.30, 22.04, 69.47, 22.05);
ctx.qcurve_to(71.28, 22.65, 71.42, 22.76);
ctx.qcurve_to(71.56, 22.86, 71.69, 22.98);
ctx.qcurve_to(72.44, 23.95, 72.51, 24.11);
ctx.qcurve_to(72.59, 24.27, 72.65, 24.43);
ctx.qcurve_to(72.80, 24.85, 72.84, 25.30);
ctx.qcurve_to(72.87, 25.71, 72.96, 26.60);
ctx.qcurve_to(73.95, 36.93, 72.33, 41.50);
ctx.qcurve_to(68.82, 51.44, 56.28, 47.67);
ctx.qcurve_to(55.81, 47.53, 55.39, 47.26);
ctx.qcurve_to(54.97, 46.99, 54.63, 46.61);
ctx.move_to(60.00, 41.89);
ctx.line_to(57.32, 44.25);
ctx.line_to(58.35, 40.83);
ctx.qcurve_to(64.36, 42.64, 65.60, 39.12);
ctx.qcurve_to(66.69, 36.04, 65.85, 27.28);
ctx.qcurve_to(65.76, 26.37, 65.72, 25.94);
ctx.line_to(69.28, 25.62);
ctx.line_to(70.47, 28.99);
ctx.qcurve_to(70.06, 29.13, 69.19, 29.43);
ctx.qcurve_to(60.88, 32.31, 58.54, 34.58);
ctx.qcurve_to(55.85, 37.18, 60.00, 41.89);
ctx.fill();

ctx.set_fill(0xff319e40);
ctx.begin_path();
ctx.move_to(57.32, 44.25);
ctx.bcurve_to(47.01, 32.54, 61.60, 28.33, 69.28, 25.62);
ctx.bcurve_to(70.01, 33.73, 72.25, 48.75, 57.32, 44.25);
ctx.fill();

ctx.set_fill(0xff288034);
ctx.begin_path();
ctx.move_to(56.26, 48.16);
ctx.qcurve_to(69.09, 45.59, 71.66, 55.82);
ctx.qcurve_to(72.84, 60.52, 70.88, 70.71);
ctx.qcurve_to(70.71, 71.59, 70.63, 72.00);
ctx.qcurve_to(70.60, 72.17, 70.55, 72.34);
ctx.qcurve_to(70.37, 72.83, 70.29, 72.99);
ctx.qcurve_to(70.21, 73.15, 70.11, 73.29);
ctx.qcurve_to(70.02, 73.44, 69.91, 73.58);
ctx.qcurve_to(69.55, 73.96, 69.42, 74.08);
ctx.qcurve_to(69.28, 74.19, 69.14, 74.29);
ctx.qcurve_to(68.99, 74.39, 68.84, 74.47);
ctx.qcurve_to(68.03, 74.80, 67.86, 74.84);
ctx.qcurve_to(67.69, 74.87, 67.51, 74.89);
ctx.qcurve_to(66.64, 74.89, 66.46, 74.85);
ctx.qcurve_to(66.03, 74.77, 65.62, 74.58);
ctx.qcurve_to(65.24, 74.41, 64.43, 74.04);
ctx.qcurve_to(54.99, 69.73, 51.84, 66.05);
ctx.qcurve_to(44.99, 58.03, 54.52, 49.06);
ctx.qcurve_to(54.88, 48.72, 55.33, 48.49);
ctx.qcurve_to(55.77, 48.26, 56.26, 48.16);
ctx.move_to(57.67, 55.17);
ctx.line_to(56.97, 51.67);
ctx.line_to(59.41, 54.27);
ctx.qcurve_to(54.84, 58.57, 57.27, 61.41);
ctx.qcurve_to(59.39, 63.89, 67.39, 67.54);
ctx.qcurve_to(68.22, 67.92, 68.62, 68.10);
ctx.line_to(67.12, 71.34);
ctx.line_to(63.61, 70.69);
ctx.qcurve_to(63.69, 70.26, 63.86, 69.36);
ctx.qcurve_to(65.52, 60.72, 64.73, 57.56);
ctx.qcurve_to(63.82, 53.93, 57.67, 55.17);
ctx.fill();

ctx.set_fill(0xff319e40);
ctx.begin_path();
ctx.move_to(56.97, 51.67);
ctx.bcurve_to(72.26, 48.60, 68.62, 63.34, 67.12, 71.34);
ctx.bcurve_to(59.73, 67.92, 45.60, 62.35, 56.97, 51.67);
ctx.fill();

ctx.set_fill(0xff288034);
ctx.begin_path();
ctx.move_to(54.10, 48.80);
ctx.qcurve_to(49.92, 61.20, 39.78, 58.30);
ctx.qcurve_to(35.12, 56.97, 27.27, 50.18);
ctx.qcurve_to(26.59, 49.59, 26.28, 49.32);
ctx.qcurve_to(26.14, 49.21, 26.02, 49.08);
ctx.qcurve_to(25.69, 48.68, 25.59, 48.53);
ctx.qcurve_to(25.50, 48.38, 25.42, 48.23);
ctx.qcurve_to(25.34, 48.07, 25.27, 47.91);
ctx.qcurve_to(25.21, 47.74, 25.16, 47.58);
ctx.qcurve_to(25.12, 47.41, 25.08, 47.23);
ctx.qcurve_to(25.05, 47.06, 25.04, 46.89);
ctx.qcurve_to(25.03, 46.71, 25.03, 46.54);
ctx.qcurve_to(25.03, 46.36, 25.06, 46.19);
ctx.qcurve_to(25.08, 46.01, 25.11, 45.84);
ctx.qcurve_to(25.15, 45.67, 25.21, 45.50);
ctx.qcurve_to(25.26, 45.34, 25.33, 45.17);
ctx.qcurve_to(25.40, 45.01, 25.49, 44.86);
ctx.qcurve_to(26.18, 43.95, 26.54, 43.69);
ctx.qcurve_to(26.88, 43.45, 27.61, 42.93);
ctx.qcurve_to(36.06, 36.91, 40.83, 36.02);
ctx.qcurve_to(51.20, 34.10, 54.20, 46.84);
ctx.qcurve_to(54.31, 47.32, 54.29, 47.82);
ctx.qcurve_to(54.26, 48.32, 54.10, 48.80);
ctx.move_to(47.34, 46.51);
ctx.line_to(50.72, 47.66);
ctx.line_to(47.24, 48.47);
ctx.qcurve_to(45.81, 42.36, 42.13, 43.05);
ctx.qcurve_to(38.92, 43.64, 31.76, 48.75);
ctx.qcurve_to(31.01, 49.28, 30.66, 49.53);
ctx.line_to(28.60, 46.61);
ctx.line_to(30.92, 43.90);
ctx.qcurve_to(31.25, 44.18, 31.94, 44.78);
ctx.qcurve_to(38.60, 50.54, 41.74, 51.43);
ctx.qcurve_to(45.33, 52.46, 47.34, 46.51);
ctx.fill();

ctx.set_fill(0xff319e40);
ctx.begin_path();
ctx.move_to(50.72, 47.66);
ctx.bcurve_to(45.73, 62.44, 34.79, 51.91, 28.60, 46.61);
ctx.bcurve_to(35.26, 41.92, 47.15, 32.47, 50.72, 47.66);
ctx.fill();

ctx.set_fill(0xff288034);
ctx.begin_path();
ctx.move_to(55.58, 43.32);
ctx.qcurve_to(56.78, 35.52, 64.12, 29.92);
ctx.qcurve_to(64.21, 29.86, 64.29, 29.80);
ctx.qcurve_to(64.38, 29.75, 64.48, 29.70);
ctx.qcurve_to(64.57, 29.66, 64.67, 29.62);
ctx.qcurve_to(64.77, 29.58, 64.87, 29.56);
ctx.qcurve_to(65.18, 29.50, 65.29, 29.49);
ctx.qcurve_to(66.92, 30.09, 66.99, 30.17);
ctx.qcurve_to(67.06, 30.25, 67.12, 30.33);
ctx.qcurve_to(67.19, 30.41, 67.24, 30.50);
ctx.qcurve_to(67.30, 30.59, 67.34, 30.69);
ctx.qcurve_to(67.39, 30.78, 67.43, 30.88);
ctx.qcurve_to(67.47, 30.98, 67.49, 31.08);
ctx.qcurve_to(67.52, 31.18, 67.53, 31.29);
ctx.qcurve_to(67.55, 31.39, 67.56, 31.50);
ctx.qcurve_to(60.76, 37.88, 59.82, 43.98);
ctx.qcurve_to(59.80, 44.08, 59.78, 44.18);
ctx.qcurve_to(59.68, 44.48, 59.63, 44.58);
ctx.qcurve_to(59.59, 44.67, 59.53, 44.76);
ctx.qcurve_to(59.21, 45.18, 59.13, 45.25);
ctx.qcurve_to(58.51, 45.64, 58.41, 45.67);
ctx.qcurve_to(58.31, 45.71, 58.21, 45.73);
ctx.qcurve_to(58.11, 45.76, 58.00, 45.77);
ctx.qcurve_to(57.90, 45.79, 57.79, 45.79);
ctx.qcurve_to(57.48, 45.78, 57.37, 45.77);
ctx.qcurve_to(57.27, 45.75, 57.17, 45.73);
ctx.qcurve_to(56.87, 45.63, 56.77, 45.58);
ctx.move_to(58.63, 50.63);
ctx.qcurve_to(64.79, 55.57, 65.97, 64.72);
ctx.qcurve_to(65.98, 64.83, 65.99, 64.93);
ctx.qcurve_to(65.99, 65.04, 65.98, 65.14);
ctx.qcurve_to(65.60, 66.22, 65.54, 66.30);
ctx.qcurve_to(65.48, 66.39, 65.40, 66.46);
ctx.qcurve_to(65.33, 66.54, 65.25, 66.61);
ctx.qcurve_to(65.17, 66.68, 65.09, 66.74);
ctx.qcurve_to(65.00, 66.80, 64.91, 66.85);
ctx.qcurve_to(64.82, 66.91, 64.72, 66.95);
ctx.qcurve_to(64.22, 67.11, 64.12, 67.12);
ctx.qcurve_to(64.01, 67.13, 63.91, 67.14);
ctx.qcurve_to(63.18, 67.04, 63.08, 67.00);
ctx.qcurve_to(62.98, 66.96, 62.89, 66.91);
ctx.qcurve_to(62.30, 66.48, 62.23, 66.40);
ctx.qcurve_to(62.16, 66.32, 62.10, 66.24);
ctx.qcurve_to(62.04, 66.15, 61.98, 66.06);
ctx.qcurve_to(61.93, 65.97, 61.89, 65.87);
ctx.qcurve_to(61.73, 65.37, 61.72, 65.27);
ctx.qcurve_to(60.76, 57.83, 55.95, 53.97);
ctx.qcurve_to(55.87, 53.91, 55.79, 53.83);
ctx.qcurve_to(55.72, 53.76, 55.65, 53.68);
ctx.qcurve_to(55.58, 53.60, 55.52, 53.51);
ctx.qcurve_to(55.46, 53.42, 55.41, 53.33);
ctx.qcurve_to(55.36, 53.24, 55.32, 53.14);
ctx.qcurve_to(55.15, 52.43, 55.15, 52.32);
ctx.qcurve_to(55.17, 52.01, 55.19, 51.91);
ctx.qcurve_to(55.21, 51.80, 55.24, 51.70);
ctx.qcurve_to(55.34, 51.40, 55.39, 51.31);
ctx.qcurve_to(55.44, 51.22, 55.50, 51.13);
ctx.qcurve_to(55.56, 51.04, 55.62, 50.96);
ctx.qcurve_to(55.69, 50.88, 55.76, 50.80);
ctx.qcurve_to(56.17, 50.47, 56.26, 50.42);
ctx.qcurve_to(56.95, 50.18, 57.06, 50.17);
ctx.qcurve_to(57.16, 50.16, 57.27, 50.16);
ctx.qcurve_to(57.37, 50.16, 57.48, 50.17);
ctx.qcurve_to(57.58, 50.17, 57.69, 50.19);
ctx.qcurve_to(57.79, 50.21, 57.89, 50.24);
ctx.qcurve_to(58.37, 50.45, 58.46, 50.51);
ctx.qcurve_to(58.55, 50.56, 58.63, 50.63);
ctx.move_to(50.78, 49.62);
ctx.qcurve_to(43.42, 52.48, 34.91, 48.92);
ctx.qcurve_to(34.81, 48.88, 34.72, 48.83);
ctx.qcurve_to(34.63, 48.78, 34.54, 48.72);
ctx.qcurve_to(34.45, 48.67, 34.37, 48.60);
ctx.qcurve_to(34.14, 48.38, 34.07, 48.30);
ctx.qcurve_to(34.01, 48.22, 33.95, 48.13);
ctx.qcurve_to(33.89, 48.04, 33.84, 47.95);
ctx.qcurve_to(33.79, 47.86, 33.75, 47.76);
ctx.qcurve_to(33.71, 47.66, 33.68, 47.56);
ctx.qcurve_to(33.65, 47.46, 33.63, 47.36);
ctx.qcurve_to(33.59, 47.04, 33.59, 46.94);
ctx.qcurve_to(34.15, 45.50, 34.22, 45.43);
ctx.qcurve_to(34.30, 45.35, 34.38, 45.29);
ctx.qcurve_to(34.46, 45.22, 34.55, 45.16);
ctx.qcurve_to(34.64, 45.10, 34.73, 45.05);
ctx.qcurve_to(34.82, 45.00, 34.92, 44.96);
ctx.qcurve_to(35.02, 44.92, 35.12, 44.89);
ctx.qcurve_to(35.22, 44.86, 35.32, 44.84);
ctx.qcurve_to(36.46, 44.93, 36.56, 44.97);
ctx.qcurve_to(43.48, 47.86, 49.23, 45.62);
ctx.qcurve_to(49.33, 45.59, 49.43, 45.56);
ctx.qcurve_to(49.53, 45.53, 49.63, 45.51);
ctx.qcurve_to(49.95, 45.48, 50.05, 45.48);
ctx.qcurve_to(51.07, 49.48, 50.98, 49.53);
ctx.qcurve_to(50.88, 49.58, 50.78, 49.62);
ctx.fill();
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
//         case PetalID::kWax: {
//        uint32_t base_color = 0xfff7ce2f;   
//     SeedGenerator gen(std::floor(r) * 1951264 + 295726);
//     ctx.set_fill(base_color);
//     ctx.set_stroke(Renderer::HSV(base_color, 0.8));
//     ctx.set_line_width(10);
//     ctx.round_line_cap();
//     ctx.round_line_join();
//     ctx.begin_path();
//     float deflection = r * 0.1;
//     ctx.move_to(r + gen.binext() * deflection, gen.binext() * deflection);
//     uint32_t sides = 6;
//     for (uint32_t i = 1; i < sides; ++i) {
//         float angle = 2 * M_PI * i / sides;
//         ctx.line_to(cosf(angle) * r + gen.binext() * deflection,
//                     sinf(angle) * r + gen.binext() * deflection);
//     }
//     ctx.close_path();
//     ctx.fill();
//     ctx.stroke();
//     break;
// }
        case PetalID::kWax: {
    uint32_t base_color = 0xfff7ce2f;
    float start = (float)M_PI / 6.0f; // flat side down
    ctx.set_fill(base_color);
    ctx.set_stroke(Renderer::HSV(base_color, 0.8f));
    ctx.set_line_width(r / 5);
    ctx.round_line_cap();
    ctx.round_line_join();
    ctx.begin_path();
    ctx.move_to(cosf(start) * r, sinf(start) * r);
    for (uint32_t i = 1; i < 6; ++i) {
        float angle = start + i * (float)M_PI / 3.0f;
        ctx.line_to(cosf(angle) * r, sinf(angle) * r);
    }
    ctx.close_path();
    ctx.fill();
    ctx.stroke();
    break;
}
        case PetalID::kHoney: {
    uint32_t base_color = 0xfff7ce2f; // same palette as Wax
    float start = (float)M_PI / 6.0f; // flat side down
    ctx.set_fill(base_color);
    ctx.set_stroke(Renderer::HSV(base_color, 0.8f));
    ctx.set_line_width(3);
    ctx.round_line_cap();
    ctx.round_line_join();
    ctx.begin_path();
    ctx.move_to(cosf(start) * r, sinf(start) * r);
    for (uint32_t i = 1; i < 6; ++i) {
        float angle = start + i * (float)M_PI / 3.0f;
        ctx.line_to(cosf(angle) * r, sinf(angle) * r);
    }
    ctx.close_path();
    ctx.fill();
    ctx.stroke();
    break;
}
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
        case PetalID::kTriFaster:
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
        //case PetalID::kEntennae:
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
        case PetalID::kFatPeas:
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
            uint32_t s = 3;
            ctx.move_to(r, 0);
            for (uint32_t i = 1; i <= s; ++i) {
                float angle = i * 2 * M_PI / s;
                float angle2 = angle - M_PI / s;
                ctx.qcurve_to(2 * r * cosf(angle2), 2 * r * sinf(angle2), r * cosf(angle), r * sinf(angle));
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
            ctx.fill(1);
            break;
        }
        case PetalID::kLotus:
            ctx.scale(r / 10);
            ctx.set_fill(0xffce76db);
            ctx.set_stroke(0xffa760b1);
            ctx.set_line_width(2);
            ctx.round_line_cap();
            ctx.round_line_join();
            ctx.begin_path();
            ctx.move_to(0.00, -10.00);
            ctx.bcurve_to(1.44, -7.11, 2.05, -5.89, 1.83, -4.41);
            ctx.bcurve_to(2.72, -5.62, 4.01, -6.05, 7.07, -7.07);
            ctx.bcurve_to(6.05, -4.01, 5.62, -2.72, 4.41, -1.83);
            ctx.bcurve_to(5.89, -2.05, 7.11, -1.44, 10.00, 0.00);
            ctx.bcurve_to(7.11, 1.44, 5.89, 2.05, 4.41, 1.83);
            ctx.bcurve_to(5.62, 2.72, 6.05, 4.01, 7.07, 7.07);
            ctx.bcurve_to(4.01, 6.05, 2.72, 5.62, 1.83, 4.41);
            ctx.bcurve_to(2.05, 5.89, 1.44, 7.11, 0.00, 10.00);
            ctx.bcurve_to(-1.44, 7.11, -2.05, 5.89, -1.83, 4.41);
            ctx.bcurve_to(-2.72, 5.62, -4.01, 6.05, -7.07, 7.07);
            ctx.bcurve_to(-6.05, 4.01, -5.62, 2.72, -4.41, 1.83);
            ctx.bcurve_to(-5.89, 2.05, -7.11, 1.44, -10.00, 0.00);
            ctx.bcurve_to(-7.11, -1.44, -5.89, -2.05, -4.41, -1.83);
            ctx.bcurve_to(-5.62, -2.72, -6.05, -4.01, -7.07, -7.07);
            ctx.bcurve_to(-4.01, -6.05, -2.72, -5.62, -1.83, -4.41);
            ctx.bcurve_to(-2.05, -5.89, -1.44, -7.11, 0.00, -10.00);
            ctx.fill();
            ctx.stroke();
            ctx.set_fill(0xffa760b1);
            ctx.begin_path();
            ctx.arc(0,0,1.74);
            ctx.fill();
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
        case PetalID::kCutter:
            ctx.set_fill(0xff111111);
            ctx.begin_path();
            ctx.arc(0,0,25);
            ctx.move_to(24.748737335205078,24.748737335205078);
            ctx.qcurve_to(9.899494171142578,23.899494171142578,-0.0000015298985545086907,35);
            ctx.qcurve_to(-9.899496078491211,23.899494171142578,-24.748737335205078,24.748737335205078);
            ctx.qcurve_to(-23.899494171142578,9.899493217468262,-35,-0.0000030597971090173814);
            ctx.qcurve_to(-23.89949607849121,-9.899496078491211,-24.74873924255371,-24.748735427856445);
            ctx.qcurve_to(-9.899496078491211,-23.899494171142578,4.173708134658227e-7,-35);
            ctx.qcurve_to(9.899493217468262,-23.89949607849121,24.748733520507812,-24.748741149902344);
            ctx.qcurve_to(23.899494171142578,-9.899494171142578,35,0.000006119594218034763);
            ctx.qcurve_to(23.899494171142578,9.899497032165527,24.748737335205078,24.748737335205078);
            ctx.fill();
            break;
        case PetalID::kYinYang:
            ctx.set_line_width(3);
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.begin_path();
            ctx.partial_arc(0,0,r,M_PI/2,3*M_PI/2,0);
            ctx.partial_arc(0,-r/2,r/2,-M_PI/2,M_PI/2,0);
            ctx.partial_arc(0,r/2,r/2,-M_PI/2,M_PI/2,1);
            ctx.fill();
            ctx.stroke();
            ctx.set_fill(0xff333333);
            ctx.set_stroke(0xff292929);
            ctx.begin_path();
            ctx.partial_arc(0,0,r,-M_PI/2,M_PI/2,0);
            ctx.partial_arc(0,r/2,r/2,M_PI/2,3*M_PI/2,0);
            ctx.partial_arc(0,-r/2,r/2,M_PI/2,3*M_PI/2,1);
            ctx.fill();
            ctx.stroke();
            ctx.set_stroke(0xffcfcfcf);
            ctx.begin_path();
            ctx.partial_arc(0,0,r,M_PI,3*M_PI/2,0);
            ctx.partial_arc(0,-r/2,r/2,-M_PI/2,M_PI/2,0);
            ctx.stroke();
            break;
        case PetalID::kYggdrasil:
            ctx.scale(r / 255);
            ctx.set_fill(0xff886d35);
            ctx.begin_path();
            ctx.move_to(-273.54, -218.49);
            ctx.qcurve_to(-284.88, -187.49, -267.08, -151.41);
            ctx.qcurve_to(-262.72, -142.57, -254.82, -136.69);
            ctx.qcurve_to(-246.93, -130.80, -237.22, -129.15);
            ctx.qcurve_to(-222.87, -126.71, -208.62, -122.97);
            ctx.qcurve_to(-212.49, -112.19, -216.13, -100.75);
            ctx.qcurve_to(-218.33, -96.85, -219.63, -92.56);
            ctx.qcurve_to(-220.93, -88.27, -221.28, -83.80);
            ctx.qcurve_to(-229.91, -54.12, -240.77, -7.55);
            ctx.qcurve_to(-244.62, 8.97, -235.66, 23.37);
            ctx.qcurve_to(-226.71, 37.78, -210.19, 41.64);
            ctx.line_to(-199.07, 44.23);
            ctx.line_to(-203.90, 73.81);
            ctx.qcurve_to(-203.91, 73.87, -203.91, 73.93);
            ctx.qcurve_to(-203.92, 73.98, -203.93, 74.04);
            ctx.qcurve_to(-205.20, 82.09, -203.29, 90.01);
            ctx.qcurve_to(-201.38, 97.93, -196.58, 104.52);
            ctx.qcurve_to(-191.79, 111.10, -184.84, 115.35);
            ctx.qcurve_to(-177.88, 119.60, -169.84, 120.87);
            ctx.line_to(-168.30, 121.11);
            ctx.line_to(-169.08, 125.06);
            ctx.qcurve_to(-169.09, 125.10, -169.10, 125.14);
            ctx.qcurve_to(-169.11, 125.18, -169.11, 125.22);
            ctx.qcurve_to(-170.67, 133.22, -169.04, 141.20);
            ctx.qcurve_to(-167.42, 149.18, -162.86, 155.94);
            ctx.qcurve_to(-158.30, 162.69, -151.51, 167.19);
            ctx.qcurve_to(-144.72, 171.69, -136.72, 173.24);
            ctx.line_to(-118.37, 176.80);
            ctx.qcurve_to(-118.31, 176.82, -118.25, 176.83);
            ctx.qcurve_to(-118.18, 176.84, -118.12, 176.85);
            ctx.qcurve_to(-117.31, 178.64, -116.33, 180.35);
            ctx.qcurve_to(-115.35, 182.05, -114.21, 183.65);
            ctx.qcurve_to(-109.49, 190.30, -102.59, 194.63);
            ctx.qcurve_to(-95.69, 198.96, -87.66, 200.32);
            ctx.line_to(-69.22, 203.45);
            ctx.qcurve_to(-68.74, 203.53, -68.27, 203.60);
            ctx.qcurve_to(-67.79, 203.67, -67.31, 203.73);
            ctx.qcurve_to(-63.19, 213.00, -55.22, 219.27);
            ctx.qcurve_to(-47.25, 225.55, -37.27, 227.38);
            ctx.line_to(-18.90, 230.76);
            ctx.qcurve_to(-14.56, 231.55, -10.15, 231.41);
            ctx.qcurve_to(-5.73, 231.26, -1.46, 230.18);
            ctx.qcurve_to(2.41, 233.19, 6.88, 235.20);
            ctx.qcurve_to(11.34, 237.21, 16.16, 238.11);
            ctx.line_to(34.53, 241.54);
            ctx.qcurve_to(34.57, 241.55, 34.60, 241.56);
            ctx.qcurve_to(34.64, 241.56, 34.68, 241.57);
            ctx.qcurve_to(37.13, 242.02, 39.62, 242.17);
            ctx.qcurve_to(42.11, 242.32, 44.60, 242.16);
            ctx.qcurve_to(50.12, 249.01, 57.96, 252.99);
            ctx.qcurve_to(65.80, 256.97, 74.58, 257.39);
            ctx.line_to(93.24, 258.26);
            ctx.qcurve_to(94.95, 258.34, 96.66, 258.28);
            ctx.qcurve_to(98.37, 258.21, 100.07, 258.01);
            ctx.qcurve_to(105.67, 262.39, 112.42, 264.63);
            ctx.qcurve_to(119.17, 266.87, 126.27, 266.70);
            ctx.line_to(144.96, 266.26);
            ctx.line_to(144.99, 266.26);
            ctx.qcurve_to(146.79, 266.22, 148.59, 266.01);
            ctx.qcurve_to(150.38, 265.81, 152.15, 265.45);
            ctx.qcurve_to(158.42, 269.45, 165.70, 270.98);
            ctx.qcurve_to(172.97, 272.52, 180.32, 271.40);
            ctx.line_to(198.79, 268.59);
            ctx.line_to(198.83, 268.58);
            ctx.qcurve_to(215.60, 266.01, 225.64, 252.34);
            ctx.qcurve_to(235.68, 238.66, 233.11, 221.89);
            ctx.qcurve_to(232.88, 220.35, 232.63, 218.81);
            ctx.line_to(248.18, 211.12);
            ctx.line_to(248.21, 211.11);
            ctx.qcurve_to(263.41, 203.59, 268.84, 187.51);
            ctx.qcurve_to(274.27, 171.44, 266.74, 156.23);
            ctx.qcurve_to(262.68, 148.04, 257.67, 140.39);
            ctx.qcurve_to(258.08, 139.88, 258.48, 139.35);
            ctx.qcurve_to(258.87, 138.83, 259.25, 138.29);
            ctx.line_to(269.93, 122.97);
            ctx.qcurve_to(277.99, 111.43, 277.25, 97.36);
            ctx.qcurve_to(276.50, 83.30, 267.27, 72.67);
            ctx.qcurve_to(267.91, 71.51, 268.47, 70.30);
            ctx.qcurve_to(269.04, 69.10, 269.52, 67.86);
            ctx.line_to(276.33, 50.46);
            ctx.line_to(276.35, 50.40);
            ctx.qcurve_to(279.31, 42.81, 279.14, 34.67);
            ctx.qcurve_to(278.97, 26.52, 275.69, 19.06);
            ctx.qcurve_to(272.55, 11.90, 266.98, 6.40);
            ctx.qcurve_to(261.41, 0.89, 254.21, -2.17);
            ctx.qcurve_to(254.51, -3.41, 254.73, -4.66);
            ctx.qcurve_to(254.96, -5.92, 255.10, -7.18);
            ctx.line_to(257.23, -25.75);
            ctx.qcurve_to(258.22, -34.46, 255.57, -42.82);
            ctx.qcurve_to(252.91, -51.18, 247.06, -57.71);
            ctx.qcurve_to(247.37, -59.47, 247.53, -61.25);
            ctx.qcurve_to(247.68, -63.03, 247.68, -64.81);
            ctx.line_to(247.68, -83.54);
            ctx.qcurve_to(247.67, -96.77, 239.92, -107.49);
            ctx.qcurve_to(232.17, -118.21, 219.62, -122.38);
            ctx.line_to(219.52, -129.09);
            ctx.qcurve_to(219.26, -146.05, 207.09, -157.86);
            ctx.qcurve_to(194.92, -169.68, 177.96, -169.43);
            ctx.qcurve_to(172.94, -169.35, 167.85, -169.16);
            ctx.line_to(167.84, -169.31);
            ctx.line_to(167.84, -169.34);
            ctx.qcurve_to(167.26, -186.29, 154.87, -197.87);
            ctx.qcurve_to(142.47, -209.45, 125.51, -208.88);
            ctx.qcurve_to(117.74, -208.61, 109.85, -208.00);
            ctx.qcurve_to(106.05, -222.95, 93.34, -231.68);
            ctx.qcurve_to(80.63, -240.40, 65.31, -238.58);
            ctx.qcurve_to(45.33, -236.20, 25.95, -232.73);
            ctx.qcurve_to(22.07, -247.33, 9.63, -255.91);
            ctx.qcurve_to(-2.81, -264.48, -17.83, -262.93);
            ctx.qcurve_to(-44.96, -260.13, -75.59, -250.78);
            ctx.qcurve_to(-79.88, -250.32, -83.97, -248.97);
            ctx.qcurve_to(-88.07, -247.62, -91.80, -245.44);
            ctx.qcurve_to(-103.36, -241.40, -116.91, -235.99);
            ctx.qcurve_to(-150.38, -254.09, -185.96, -269.59);
            ctx.qcurve_to(-192.91, -272.62, -200.49, -272.95);
            ctx.qcurve_to(-208.07, -273.29, -215.27, -270.89);
            ctx.qcurve_to(-216.07, -270.62, -216.85, -270.33);
            ctx.qcurve_to(-217.64, -270.03, -218.41, -269.70);
            ctx.qcurve_to(-261.32, -251.90, -273.54, -218.49);
            ctx.fill();
            ctx.set_fill(0xffa88642);
            ctx.begin_path();
            ctx.move_to(-230.34, -169.53);
            ctx.bcurve_to(-242.75, -194.66, -239.90, -216.60, -202.31, -232.03);
            ctx.line_to(-202.31, -232.03);
            ctx.bcurve_to(-175.35, -220.29, -147.49, -206.55, -119.56, -190.60);
            ctx.bcurve_to(-103.63, -197.35, -87.58, -203.79, -71.38, -209.13);
            ctx.line_to(-71.16, -210.06);
            ctx.bcurve_to(-70.64, -209.95, -70.14, -209.84, -69.63, -209.72);
            ctx.bcurve_to(-51.16, -215.71, -32.51, -220.24, -13.63, -222.19);
            ctx.line_to(-11.69, -203.60);
            ctx.bcurve_to(-18.65, -202.88, -25.65, -201.75, -32.69, -200.28);
            ctx.bcurve_to(-18.86, -196.28, -5.65, -191.88, 6.97, -187.13);
            ctx.bcurve_to(28.05, -191.87, 49.12, -195.40, 70.16, -197.91);
            ctx.line_to(72.38, -179.35);
            ctx.bcurve_to(60.85, -177.97, 49.33, -176.28, 37.81, -174.25);
            ctx.bcurve_to(47.45, -169.82, 56.68, -165.19, 65.50, -160.31);
            ctx.bcurve_to(85.48, -164.46, 105.90, -167.22, 126.91, -167.94);
            ctx.line_to(127.53, -149.28);
            ctx.bcurve_to(114.77, -148.85, 102.19, -147.59, 89.72, -145.69);
            ctx.bcurve_to(100.48, -138.62, 110.54, -131.19, 119.88, -123.44);
            ctx.bcurve_to(139.03, -126.34, 158.55, -128.18, 178.56, -128.47);
            ctx.line_to(178.85, -109.78);
            ctx.bcurve_to(165.10, -109.58, 151.54, -108.61, 138.13, -107.03);
            ctx.bcurve_to(146.65, -98.70, 154.45, -90.07, 161.53, -81.16);
            ctx.bcurve_to(176.57, -82.67, 191.64, -83.48, 206.72, -83.50);
            ctx.line_to(206.72, -64.81);
            ctx.bcurve_to(195.94, -64.80, 185.15, -64.36, 174.35, -63.53);
            ctx.bcurve_to(178.70, -57.00, 182.72, -50.36, 186.38, -43.59);
            ctx.bcurve_to(188.32, -39.99, 190.16, -36.35, 191.91, -32.68);
            ctx.bcurve_to(200.12, -32.12, 208.33, -31.35, 216.53, -30.40);
            ctx.line_to(214.41, -11.84);
            ctx.bcurve_to(209.66, -12.39, 204.90, -12.86, 200.16, -13.28);
            ctx.bcurve_to(204.99, -0.35, 208.65, 12.87, 211.19, 26.25);
            ctx.bcurve_to(220.15, 28.94, 229.14, 32.00, 238.19, 35.53);
            ctx.line_to(231.38, 52.94);
            ctx.bcurve_to(225.61, 50.69, 219.89, 48.60, 214.19, 46.72);
            ctx.bcurve_to(215.56, 59.97, 215.87, 73.34, 215.16, 86.75);
            ctx.bcurve_to(222.29, 90.45, 229.37, 94.66, 236.35, 99.53);
            ctx.line_to(225.66, 114.84);
            ctx.bcurve_to(221.59, 112.00, 217.47, 109.40, 213.31, 107.00);
            ctx.bcurve_to(211.78, 118.85, 209.49, 130.70, 206.41, 142.47);
            ctx.bcurve_to(215.97, 151.68, 224.08, 162.38, 230.03, 174.41);
            ctx.line_to(213.28, 182.69);
            ctx.bcurve_to(209.82, 175.69, 205.38, 169.15, 200.19, 163.16);
            ctx.bcurve_to(196.27, 174.68, 191.60, 186.11, 186.25, 197.37);
            ctx.bcurve_to(188.93, 207.61, 191.06, 217.85, 192.63, 228.09);
            ctx.line_to(174.16, 230.91);
            ctx.bcurve_to(172.83, 222.25, 171.08, 213.58, 168.91, 204.91);
            ctx.bcurve_to(160.54, 204.65, 152.23, 204.20, 144.00, 203.56);
            ctx.bcurve_to(143.81, 210.75, 143.82, 218.00, 144.00, 225.31);
            ctx.line_to(125.31, 225.75);
            ctx.bcurve_to(125.12, 217.76, 125.12, 209.77, 125.34, 201.81);
            ctx.bcurve_to(115.61, 200.71, 106.02, 199.31, 96.53, 197.66);
            ctx.bcurve_to(95.94, 204.19, 95.46, 210.76, 95.16, 217.34);
            ctx.line_to(76.50, 216.47);
            ctx.bcurve_to(76.85, 208.96, 77.40, 201.51, 78.09, 194.10);
            ctx.bcurve_to(66.82, 191.67, 55.75, 188.82, 44.94, 185.60);
            ctx.bcurve_to(43.99, 190.82, 43.04, 196.05, 42.06, 201.28);
            ctx.line_to(23.69, 197.84);
            ctx.bcurve_to(24.80, 191.86, 25.88, 185.85, 26.97, 179.84);
            ctx.bcurve_to(15.32, 175.79, 3.99, 171.23, -6.97, 166.22);
            ctx.bcurve_to(-8.51, 174.30, -10.01, 182.39, -11.50, 190.47);
            ctx.line_to(-29.88, 187.10);
            ctx.bcurve_to(-28.07, 177.30, -26.25, 167.49, -24.38, 157.69);
            ctx.bcurve_to(-36.05, 151.58, -47.24, 144.90, -57.91, 137.63);
            ctx.bcurve_to(-59.44, 146.09, -60.93, 154.57, -62.38, 163.06);
            ctx.line_to(-80.81, 159.94);
            ctx.bcurve_to(-78.85, 148.41, -76.80, 136.89, -74.66, 125.38);
            ctx.bcurve_to(-84.83, 117.40, -94.38, 108.82, -103.31, 99.66);
            ctx.bcurve_to(-105.78, 111.96, -108.18, 124.28, -110.56, 136.59);
            ctx.line_to(-128.91, 133.03);
            ctx.bcurve_to(-125.64, 116.14, -122.30, 99.24, -118.84, 82.31);
            ctx.bcurve_to(-126.58, 72.92, -133.71, 63.02, -140.13, 52.59);
            ctx.bcurve_to(-141.79, 62.81, -143.39, 73.06, -145.00, 83.31);
            ctx.line_to(-163.47, 80.41);
            ctx.bcurve_to(-160.61, 62.22, -157.70, 43.99, -154.56, 25.78);
            ctx.bcurve_to(-162.84, 8.07, -169.33, -10.85, -173.78, -30.97);
            ctx.bcurve_to(-176.87, -18.73, -179.80, -6.39, -182.69, 6.00);
            ctx.line_to(-200.88, 1.75);
            ctx.bcurve_to(-194.65, -24.93, -188.20, -51.67, -180.28, -78.00);
            ctx.bcurve_to(-180.34, -78.89, -180.39, -79.77, -180.44, -80.66);
            ctx.line_to(-179.47, -80.72);
            ctx.bcurve_to(-172.47, -103.66, -164.33, -126.26, -154.19, -148.19);
            ctx.bcurve_to(-179.16, -158.00, -204.57, -165.14, -230.34, -169.53);
            ctx.move_to(-25.91, -178.72);
            ctx.bcurve_to(-39.40, -182.96, -53.58, -186.83, -68.44, -190.28);
            ctx.line_to(-68.44, -190.28);
            ctx.bcurve_to(-78.68, -186.81, -89.00, -182.86, -99.38, -178.66);
            ctx.bcurve_to(-90.90, -173.48, -82.42, -168.10, -74.00, -162.50);
            ctx.bcurve_to(-57.97, -168.72, -41.93, -174.11, -25.91, -178.72);
            ctx.move_to(5.64, -167.66);
            ctx.bcurve_to(5.64, -167.66, 5.63, -167.65, 5.63, -167.66);
            ctx.line_to(5.63, -167.66);
            ctx.bcurve_to(-14.67, -162.88, -34.94, -156.89, -55.22, -149.56);
            ctx.bcurve_to(-46.91, -143.66, -38.64, -137.56, -30.47, -131.22);
            ctx.bcurve_to(-8.04, -139.55, 14.63, -147.37, 37.84, -153.66);
            ctx.bcurve_to(27.65, -158.60, 16.92, -163.27, 5.66, -167.66);
            ctx.bcurve_to(5.65, -167.66, 5.65, -167.66, 5.64, -167.66);
            ctx.move_to(-136.94, -140.94);
            ctx.bcurve_to(-146.66, -119.98, -154.57, -98.17, -161.41, -75.81);
            ctx.bcurve_to(-159.70, -51.76, -155.24, -29.33, -148.31, -8.53);
            ctx.bcurve_to(-140.32, -49.57, -130.31, -90.46, -115.69, -130.75);
            ctx.bcurve_to(-122.72, -134.37, -129.82, -137.76, -136.94, -140.94);
            ctx.move_to(95.19, -119.13);
            ctx.bcurve_to(85.12, -126.62, 74.26, -133.80, 62.56, -140.56);
            ctx.bcurve_to(36.94, -134.78, 11.74, -126.62, -13.40, -117.53);
            ctx.bcurve_to(-5.49, -110.98, 2.33, -104.22, 10.03, -97.22);
            ctx.bcurve_to(37.95, -105.53, 66.19, -113.36, 95.19, -119.13);
            ctx.move_to(-71.44, -104.72);
            ctx.bcurve_to(-80.56, -110.77, -89.76, -116.46, -99.06, -121.75);
            ctx.bcurve_to(-115.97, -74.44, -126.56, -25.84, -135.19, 23.38);
            ctx.bcurve_to(-129.04, 35.76, -121.89, 47.43, -113.87, 58.44);
            ctx.bcurve_to(-102.36, 4.19, -89.13, -50.22, -71.44, -104.72);
            ctx.move_to(139.44, -78.47);
            ctx.bcurve_to(131.88, -87.13, 123.53, -95.53, 114.38, -103.59);
            ctx.line_to(114.38, -103.59);
            ctx.bcurve_to(84.47, -98.53, 55.16, -90.83, 25.88, -82.31);
            ctx.bcurve_to(33.85, -74.56, 41.70, -66.58, 49.34, -58.31);
            ctx.bcurve_to(79.28, -67.12, 109.31, -74.12, 139.44, -78.47);
            ctx.move_to(-22.37, -67.75);
            ctx.bcurve_to(-33.21, -76.95, -44.20, -85.58, -55.34, -93.62);
            ctx.bcurve_to(-73.50, -36.79, -86.93, 20.16, -98.75, 77.28);
            ctx.bcurve_to(-90.13, 87.00, -80.74, 96.09, -70.65, 104.56);
            ctx.bcurve_to(-59.15, 46.57, -44.45, -11.13, -22.37, -67.75);
            ctx.move_to(169.94, -34.69);
            ctx.bcurve_to(165.02, -43.79, 159.40, -52.71, 153.06, -61.41);
            ctx.bcurve_to(123.19, -57.76, 93.22, -51.30, 63.16, -42.84);
            ctx.bcurve_to(67.65, -37.64, 72.07, -32.34, 76.41, -26.94);
            ctx.bcurve_to(101.98, -31.44, 127.56, -33.67, 153.09, -33.88);
            ctx.bcurve_to(158.89, -33.92, 164.68, -33.84, 170.47, -33.69);
            ctx.bcurve_to(170.29, -34.02, 170.12, -34.35, 169.94, -34.69);
            ctx.move_to(21.07, -26.87);
            ctx.bcurve_to(11.71, -36.57, 2.23, -45.82, -7.41, -54.59);
            ctx.bcurve_to(-28.73, 1.60, -42.90, 59.16, -54.13, 117.44);
            ctx.bcurve_to(-43.57, 125.07, -32.39, 132.13, -20.63, 138.53);
            ctx.bcurve_to(-9.69, 83.54, 3.17, 28.40, 21.07, -26.87);
            ctx.move_to(179.50, -14.69);
            ctx.bcurve_to(171.49, -15.06, 163.47, -15.24, 155.47, -15.22);
            ctx.bcurve_to(133.43, -15.15, 111.42, -13.49, 89.41, -10.13);
            ctx.bcurve_to(94.86, -2.81, 100.19, 4.65, 105.34, 12.31);
            ctx.bcurve_to(107.42, 12.27, 109.50, 12.23, 111.56, 12.22);
            ctx.bcurve_to(138.67, 12.05, 164.85, 14.76, 191.00, 20.84);
            ctx.bcurve_to(188.19, 8.79, 184.37, -3.08, 179.50, -14.69);
            ctx.move_to(66.34, 24.97);
            ctx.bcurve_to(56.34, 12.32, 46.12, 0.28, 35.72, -11.22);
            ctx.line_to(35.72, -11.22);
            ctx.bcurve_to(19.22, 41.49, 7.12, 94.33, -3.31, 147.31);
            ctx.bcurve_to(7.52, 152.42, 18.76, 157.03, 30.37, 161.16);
            ctx.bcurve_to(38.68, 115.57, 47.86, 69.58, 66.34, 24.97);
            ctx.move_to(194.75, 41.00);
            ctx.bcurve_to(169.21, 34.32, 143.84, 31.17, 117.31, 30.94);
            ctx.line_to(117.31, 30.94);
            ctx.bcurve_to(123.40, 40.80, 129.23, 50.92, 134.78, 61.34);
            ctx.bcurve_to(155.37, 65.37, 176.27, 70.11, 196.84, 78.34);
            ctx.bcurve_to(197.13, 65.80, 196.46, 53.32, 194.75, 41.00);
            ctx.move_to(101.44, 73.03);
            ctx.bcurve_to(94.28, 62.46, 87.00, 52.21, 79.59, 42.28);
            ctx.bcurve_to(64.33, 82.45, 56.09, 124.45, 48.31, 167.06);
            ctx.bcurve_to(58.69, 170.21, 69.32, 172.96, 80.19, 175.34);
            ctx.bcurve_to(84.74, 140.42, 92.56, 106.48, 101.44, 73.03);
            ctx.move_to(195.59, 98.09);
            ctx.bcurve_to(179.43, 91.04, 162.71, 86.42, 145.56, 82.66);
            ctx.line_to(145.56, 82.66);
            ctx.bcurve_to(150.27, 92.50, 154.68, 102.61, 158.87, 112.94);
            ctx.bcurve_to(169.94, 117.04, 180.61, 122.52, 190.37, 129.31);
            ctx.bcurve_to(192.76, 118.91, 194.51, 108.49, 195.59, 98.09);
            ctx.move_to(136.28, 128.75);
            ctx.bcurve_to(129.43, 116.88, 122.41, 105.34, 115.28, 94.13);
            ctx.bcurve_to(108.22, 122.18, 102.23, 150.35, 98.56, 178.97);
            ctx.bcurve_to(107.69, 180.58, 116.96, 181.94, 126.34, 183.03);
            ctx.bcurve_to(127.83, 164.70, 130.88, 146.55, 136.28, 128.75);
            ctx.move_to(185.13, 148.81);
            ctx.bcurve_to(179.87, 144.69, 174.22, 141.02, 168.31, 137.81);
            ctx.bcurve_to(171.85, 147.90, 175.14, 158.22, 178.16, 168.75);
            ctx.bcurve_to(180.73, 162.13, 183.07, 155.48, 185.13, 148.81);
            ctx.move_to(154.56, 161.91);
            ctx.bcurve_to(152.92, 158.79, 151.26, 155.70, 149.59, 152.63);
            ctx.bcurve_to(147.39, 163.18, 145.89, 173.92, 145.00, 184.84);
            ctx.bcurve_to(151.12, 185.32, 157.26, 185.71, 163.47, 185.97);
            ctx.bcurve_to(160.88, 177.96, 157.91, 169.93, 154.56, 161.90);
            ctx.fill();
            break;
        case PetalID::kRice:
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(9);
            ctx.scale(r / 13);
            ctx.begin_path();
            ctx.move_to(-8,0);
            ctx.qcurve_to(0,-3.5,8,0);
            ctx.stroke();
            ctx.set_stroke(0xffffffff);
            ctx.set_line_width(5);
            ctx.stroke();
            break;
        case PetalID::kBone:
            ctx.set_fill(0xffffffff);
            ctx.set_stroke(0xffcfcfcf);
            ctx.set_line_width(5);
            ctx.scale(r / 12);
            ctx.begin_path();
            ctx.move_to(-10,-4);
            ctx.qcurve_to(0,0,10,-4);
            ctx.bcurve_to(14,-10,20,-2,14,0);
            ctx.bcurve_to(20,2,14,10,10,4);
            ctx.qcurve_to(0,0,-10,4);
            ctx.bcurve_to(-14,10,-20,2,-14,0);
            ctx.bcurve_to(-20,-2,-14,-10,-10,-4);
            ctx.stroke();
            ctx.fill();
            break;
        case PetalID::kYucca:
            ctx.set_fill(0xff74b53f);
            ctx.set_stroke(0xff5e9333);
            ctx.set_line_width(3);
            ctx.begin_path();
            ctx.move_to(14,0);
            ctx.qcurve_to(0,-12,-14,0);
            ctx.qcurve_to(0,12,14,0);
            ctx.fill();
            ctx.stroke();
            ctx.set_line_width(2);
            ctx.begin_path();
            ctx.move_to(14,0);
            ctx.qcurve_to(0,-3,-14,0);
            ctx.stroke();
            break;
        case PetalID::kCorn:
            ctx.scale(r / 10);
            ctx.set_fill(0xffffe419);
            ctx.set_stroke(0xffcfb914);
            ctx.set_line_width(2);
            ctx.begin_path();
            ctx.move_to(-5,8);
            ctx.qcurve_to(-15,-8,0,-8);
            ctx.qcurve_to(15,-8,5,8);
            ctx.qcurve_to(0,2,-5,8);
            ctx.fill();
            ctx.stroke();
            break;
        default:
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