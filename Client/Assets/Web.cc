#include <Client/Assets/Assets.hh>

#include <Shared/Helpers.hh>

#include <cmath>

void draw_web(Renderer &ctx) {
    float const r = 10;
    uint32_t const spokes = 10;
    uint32_t const levels = 5;
    ctx.set_stroke(0x60ffffff);
    ctx.round_line_cap();
    ctx.set_line_width(0.75);
    ctx.begin_path();
    for (uint32_t i = 0; i < spokes; ++i) {
        float angle = 2 * i * M_PI / spokes;
        ctx.move_to(0,0);
        ctx.line_to(r * cosf(angle), r * sinf(angle));
    }
    for (uint32_t j = 0; j < levels; ++j) {
        float curr = j * r / levels;
        float below = curr - r / (2 * levels);
        ctx.move_to(curr, 0);
        for (uint32_t i = 0; i < spokes; ++i) {
            float angle = 2 * (i + 1) * M_PI / spokes;
            float before = (2 * i + 1) * M_PI / spokes;
            ctx.qcurve_to(
                below * cosf(before), below * sinf(before),
                curr * cosf(angle), curr * sinf(angle)
            );
        }
    }
    ctx.stroke();
}
