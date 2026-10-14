#include <Client/Render/Renderer.hh>

#include <Helpers/Macros.hh>
#include <Helpers/Math.hh>
#include <Helpers/UTF8.hh>

#include <algorithm>
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif

#include <emscripten.h>

EM_JS(void, js_bind_RenderContext, (uint32_t arg0), {
    Module.ctxs[arg0].save();
});


EM_JS(void, js_bind_destroy_RenderContext, (uint32_t arg0), {
    Module.ctxs[arg0].restore();
});


EM_JS(void, js_bind_Renderer, (uint32_t arg0), {
    if (arg0 === 0) {
        Module.ctxs[arg0] = document.getElementById('canvas').getContext('2d');
    } else {
        Module.ctxs[arg0] = new OffscreenCanvas(1,1).getContext('2d');
    }
});

EM_JS(void, js_bind_destroy_Renderer, (uint32_t arg0), {
    if (arg0 == 0)
        throw new Error('Tried to delete the main context');
    Module.ctxs[arg0] = null;
    Module.availableCtxs.push(arg0);
});

EM_JS(void, js_bind_set_dimensions, (uint32_t arg0, double arg1, double arg2), {
    Module.ctxs[arg0].canvas.width = arg1;
    Module.ctxs[arg0].canvas.height = arg2;
});

EM_JS(void, js_bind_set_fill, (uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4), {
    Module.ctxs[arg0].fillStyle = "rgba("+arg3+","+arg2+","+arg1+","+arg4/255+")";
});

EM_JS(void, js_bind_set_stroke, (uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4), {
    Module.ctxs[arg0].strokeStyle = "rgba("+arg3+","+arg2+","+arg1+","+arg4/255+")";
});

EM_JS(void, js_bind_set_line_width, (uint32_t arg0, double arg1), {
    Module.ctxs[arg0].lineWidth = arg1;
});

EM_JS(void, js_bind_set_text_size, (uint32_t arg0, double arg1), {
    Module.ctxs[arg0].font = arg1 + "px Ubuntu";
});

EM_JS(void, js_bind_set_global_alpha, (uint32_t arg0, double arg1), {
    Module.ctxs[arg0].globalAlpha = arg1;
});

EM_JS(void, js_bind_round_line_cap, (uint32_t arg0), {
    Module.ctxs[arg0].lineCap = "round";
});

EM_JS(void, js_bind_round_line_join, (uint32_t arg0), {
    Module.ctxs[arg0].lineJoin = "round";
});

EM_JS(void, js_bind_center_text_align, (uint32_t arg0), {
    Module.ctxs[arg0].textAlign = "center";
});

EM_JS(void, js_bind_center_text_baseline, (uint32_t arg0), {
    Module.ctxs[arg0].textBaseline = "middle";
});

EM_JS(void, js_bind_update_transform, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4, double arg5, double arg6), {
    Module.ctxs[arg0].setTransform(arg1, arg2, arg4, arg5, arg3, arg6);
});

EM_JS(void, js_bind_begin_path, (uint32_t arg0), {
    Module.ctxs[arg0].beginPath();
});

EM_JS(void, js_bind_move_to, (uint32_t arg0, double arg1, double arg2), {
    Module.ctxs[arg0].moveTo(arg1, arg2);
});

EM_JS(void, js_bind_line_to, (uint32_t arg0, double arg1, double arg2), {
    Module.ctxs[arg0].lineTo(arg1, arg2);
});

EM_JS(void, js_bind_qcurve_to, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4), {
    Module.ctxs[arg0].quadraticCurveTo(arg1, arg2, arg3, arg4);
});

EM_JS(void, js_bind_bcurve_to, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4, double arg5, double arg6), {
    Module.ctxs[arg0].bezierCurveTo(arg1, arg2, arg3, arg4, arg5, arg6);
});

EM_JS(void, js_bind_partial_arc, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4, double arg5, uint32_t arg6), {
    Module.ctxs[arg0].arc(arg1, arg2, arg3, arg4, arg5, !!arg6);
});

EM_JS(void, js_bind_ellipse, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4, double arg5), {
    Module.ctxs[arg0].ellipse(arg1, arg2, arg3, arg4, arg5, 2 * Math.PI, 0);
});

EM_JS(void, js_bind_fill_rect, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4), {
    Module.ctxs[arg0].fillRect(arg1, arg2, arg3, arg4);
});

EM_JS(void, js_bind_stroke_rect, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4), {
    Module.ctxs[arg0].strokeRect(arg1, arg2, arg3, arg4);
});

EM_JS(void, js_bind_rect, (uint32_t arg0, double arg1, double arg2, double arg3, double arg4), {
    Module.ctxs[arg0].rect(arg1, arg2, arg3, arg4);
});

EM_JS(void, js_bind_close_path, (uint32_t arg0), {
    Module.ctxs[arg0].closePath();
});

EM_JS(void, js_bind_fill, (uint32_t arg0, uint32_t arg1), {
    Module.ctxs[arg0].fill(arg1 ? "nonzero" : "evenodd");
});

EM_JS(void, js_bind_stroke, (uint32_t arg0), {
    Module.ctxs[arg0].stroke();
});

EM_JS(void, js_bind_clip, (uint32_t arg0), {
    Module.ctxs[arg0].clip();
});

EM_JS(void, js_bind_draw_image, (uint32_t arg0, uint32_t arg1, double arg2, double arg3), {
    Module.ctxs[arg0].drawImage(Module.ctxs[arg1].canvas, arg2, arg3);
});

EM_JS(void, js_bind_fill_text, (uint32_t arg0, char const *arg1, size_t arg2), {
    Module.ctxs[arg0].fillText(Module.TextDecoder.decode(HEAPU8.subarray(arg1, arg1+arg2)),0,0);
});

EM_JS(void, js_bind_stroke_text, (uint32_t arg0, char const *arg1, size_t arg2), {
    Module.ctxs[arg0].strokeText(Module.TextDecoder.decode(HEAPU8.subarray(arg1, arg1+arg2)),0,0);
});

EM_JS(double, js_bind_get_text_size, (uint32_t arg0, char const *arg1, size_t arg2), {
    return Module.ctxs[arg0].measureText(Module.TextDecoder.decode(HEAPU8.subarray(arg1, arg1+arg2)),0,0).width;
});

std::vector<Renderer *> Renderer::renderers;
static std::vector<uint32_t> free_ids;
static uint32_t next_id = 0;

RenderContext::RenderContext() {}
RenderContext::RenderContext(Renderer *r) {
    *this = r->context;
    renderer = r;
    js_bind_RenderContext(r->id);
}

void RenderContext::reset() {
    amount = 0;
    color_filter = 0;
    clip_x = renderer->width / 2;
    clip_y = renderer->height / 2;
    //prevents premature unrendering
    clip_w = std::fmax(renderer->width, 10000.0);
    clip_h = std::fmax(renderer->height, 10000.0);
}

RenderContext::~RenderContext() {
    renderer->context = *this;
    js_bind_destroy_RenderContext(renderer->id);
}

Renderer::Renderer() : context() {
    if (free_ids.empty()) id = next_id++;
    else {
        id = free_ids.back();
        free_ids.pop_back();
    }
    js_bind_Renderer(id);
    DEBUG_ONLY(std::cout << "created canvas " << id << '\n';)
    Renderer::renderers.push_back(this);
    context.renderer = this;
    context.reset();
}

Renderer::~Renderer() {
    js_bind_destroy_Renderer(id);
    DEBUG_ONLY(std::cout << "removed canvas " << id << '\n';)
    free_ids.push_back(id);
    const auto it = std::find(renderers.begin(), renderers.end(), this);
    if (it != renderers.end()) renderers.erase(it);
}

uint32_t Renderer::HSV(uint32_t c, float v) {
    return MIX(c >> 24 << 24, c, v);
}

uint32_t Renderer::MIX(uint32_t base, uint32_t mix, float v) {
    uint8_t b = fclamp((mix & 255) * v + (base & 255) * (1 - v), 0, 255);
    uint8_t g = fclamp(((mix >> 8) & 255) * v + ((base >> 8) & 255) * (1 - v), 0, 255);
    uint8_t r = fclamp(((mix >> 16) & 255) * v + ((base >> 16) & 255) * (1 - v), 0, 255);
    uint8_t a = base >> 24;
    return (a << 24) | (r << 16) | (g << 8) | b;
}

void Renderer::reset() {
    reset_transform();
    round_line_cap();
    round_line_join();
    center_text_align();
    center_text_baseline();
    context.reset();
}

void Renderer::set_dimensions(float w, float h) {
    width = w;
    height = h;
    js_bind_set_dimensions(id, w, h);
}

void Renderer::add_color_filter(uint32_t c, float v) {
    context.color_filter = c;
    context.amount = v;
}

void Renderer::set_fill(uint32_t v) {
    v = MIX(v, context.color_filter, context.amount);
    js_bind_set_fill(id, v & 255, (v >> 8) & 255, (v >> 16) & 255, v >> 24);
}

void Renderer::set_stroke(uint32_t v) {
    v = MIX(v, context.color_filter, context.amount);
    js_bind_set_stroke(id, v & 255, (v >> 8) & 255, (v >> 16) & 255, v >> 24);
}

void Renderer::set_line_width(float v) {
    js_bind_set_line_width(id, v);
}

void Renderer::set_text_size(float v) {
    js_bind_set_text_size(id, v);
}

void Renderer::set_global_alpha(float v) {
    js_bind_set_global_alpha(id, v);
}

void Renderer::round_line_cap() {
    js_bind_round_line_cap(id);
}

void Renderer::round_line_join() {
    js_bind_round_line_join(id);
}

void Renderer::center_text_align() {
    js_bind_center_text_align(id);
}

void Renderer::center_text_baseline() {
    js_bind_center_text_baseline(id);
}

static void update_transform(Renderer *r) {
    js_bind_update_transform(r->id, r->context.transform_matrix[0], r->context.transform_matrix[1], r->context.transform_matrix[2], r->context.transform_matrix[3], r->context.transform_matrix[4], r->context.transform_matrix[5]);
}

void Renderer::set_transform(float a, float b, float c, float d, float e, float f) {
    context.transform_matrix[0] = a;
    context.transform_matrix[1] = b;
    context.transform_matrix[2] = c;
    context.transform_matrix[3] = d;
    context.transform_matrix[4] = e;
    context.transform_matrix[5] = f;
    update_transform(this);
}

void Renderer::scale(float v) {
    context.transform_matrix[0] *= v;
    context.transform_matrix[1] *= v;
    context.transform_matrix[3] *= v;
    context.transform_matrix[4] *= v;
    update_transform(this);
}

void Renderer::scale(float x, float y) {
    context.transform_matrix[0] *= x;
    context.transform_matrix[1] *= x;
    context.transform_matrix[3] *= y;
    context.transform_matrix[4] *= y;
    update_transform(this);
}

void Renderer::translate(float x, float y) {
    context.transform_matrix[2] += x * context.transform_matrix[0] + y * context.transform_matrix[3];
    context.transform_matrix[5] += y * context.transform_matrix[4] + x * context.transform_matrix[1];
    update_transform(this);
}

void Renderer::rotate(float a) {
    float cos_a = cosf(a);
    float sin_a = sinf(a);
    float original0 = context.transform_matrix[0];
    float original1 = context.transform_matrix[1];
    float original3 = context.transform_matrix[3];
    float original4 = context.transform_matrix[4];
    context.transform_matrix[0] = original0 * cos_a + original1 * -sin_a;
    context.transform_matrix[1] = original0 * sin_a + original1 * cos_a;
    context.transform_matrix[3] = original3 * cos_a + original4 * -sin_a;
    context.transform_matrix[4] = original3 * sin_a + original4 * cos_a;
    update_transform(this);
}

void Renderer::reset_transform() {
    set_transform(1,0,0,0,1,0);
}

void Renderer::begin_path() {
    js_bind_begin_path(id);
}

void Renderer::move_to(float x, float y) {
    js_bind_move_to(id, x, y);
}

void Renderer::line_to(float x, float y) {
    js_bind_line_to(id, x, y);
}

void Renderer::qcurve_to(float x, float y, float x1, float y1) {
    js_bind_qcurve_to(id, x, y, x1, y1);
}

void Renderer::bcurve_to(float x, float y, float x1, float y1, float x2, float y2) {
    js_bind_bcurve_to(id, x, y, x1, y1, x2, y2);
}

void Renderer::partial_arc(float x, float y, float r, float sa, float ea, uint8_t ccw) {
    js_bind_partial_arc(id, x, y, r, sa, ea, ccw);
}

void Renderer::arc(float x, float y, float r) {
    partial_arc(x, y, r, 0, 2 * M_PI, 0);
}

void Renderer::reverse_arc(float x, float y, float r) {
    partial_arc(x, y, r, 0, 2 * M_PI, 1);
}

void Renderer::ellipse(float x, float y, float r1, float r2, float a) {
    js_bind_ellipse(id, x, y, r1, r2, a);
}

void Renderer::ellipse(float x, float y, float r1, float r2) {
    ellipse(x, y, r1, r2, 0);
}

void Renderer::fill_rect(float x, float y, float w, float h) {
    js_bind_fill_rect(id, x, y, w, h);
}

void Renderer::stroke_rect(float x, float y, float w, float h) {
    js_bind_stroke_rect(id, x, y, w, h);
}

void Renderer::rect(float x, float y, float w, float h) {
    js_bind_rect(id, x, y, w, h);
}

void Renderer::round_rect(float x, float y, float w, float h, float r) {
    move_to(x + r, y);
    line_to(x + w - r, y);
    qcurve_to(x + w, y, x + w, y + r);
    line_to(x + w, y + h - r);
    qcurve_to(x + w, y + h, x + w - r, y + h);
    line_to(x + r, y + h);
    qcurve_to(x, y + h, x, y + h - r);
    line_to(x, y + r);
    qcurve_to(x, y, x + r, y);
}

void Renderer::close_path() {
    js_bind_close_path(id);
}

void Renderer::fill(uint8_t o) {
    js_bind_fill(id, o);
}

void Renderer::stroke() {
    js_bind_stroke(id);
}

void Renderer::clip() {
    js_bind_clip(id);
}

void Renderer::clip_rect(float x, float y, float w, float h) {
    //assumes axis oriented scaling
    context.clip_x = x * context.transform_matrix[0] + context.transform_matrix[2];
    context.clip_w = w * context.transform_matrix[0];
    context.clip_y = y * context.transform_matrix[4] + context.transform_matrix[5];
    context.clip_h = h * context.transform_matrix[4];
    begin_path();
    rect(x-w/2,y-h/2,w,h);
    clip();
}

void Renderer::draw_image(Renderer &ctx) {
    js_bind_draw_image(id, ctx.id, -ctx.width / 2, -ctx.height / 2);
}

void Renderer::fill_text(std::string_view text) {
    js_bind_fill_text(id, text.data(), text.size());
}

void Renderer::stroke_text(std::string_view text) {
    js_bind_stroke_text(id, text.data(), text.size());
}

void Renderer::draw_text(std::string_view text, struct TextArgs const args) {
    set_fill(args.fill);
    set_stroke(args.stroke);
    set_text_size(args.size);
    if (args.stroke_scale > 0) {
        set_line_width(args.size * args.stroke_scale);
        stroke_text(text);
    }
    fill_text(text);
}

float Renderer::get_text_size(std::string_view text) {
    return js_bind_get_text_size(id, text.data(), text.size());
}

//precalculated ascii for standard Ubuntu font, only serves as an approximation (will fail for certain scripts)
constexpr float CHAR_WIDTHS[128] = {0,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0,0.24,0.24,0.24,0.24,0.24,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0,0.5,0.5,0.24,0.286,0.465,0.699,0.568,0.918,0.705,0.247,0.356,0.356,0.502,0.568,0.246,0.34,0.246,0.437,0.568,0.568,0.568,0.568,0.568,0.568,0.568,0.568,0.568,0.568,0.246,0.246,0.568,0.568,0.568,0.455,0.974,0.721,0.672,0.648,0.737,0.606,0.574,0.702,0.734,0.316,0.529,0.684,0.563,0.897,0.756,0.79,0.644,0.79,0.667,0.582,0.614,0.707,0.722,0.948,0.675,0.661,0.61,0.371,0.437,0.371,0.568,0.5,0.286,0.553,0.604,0.5,0.604,0.584,0.422,0.594,0.589,0.289,0.289,0.579,0.316,0.862,0.589,0.607,0.604,0.604,0.422,0.485,0.444,0.589,0.55,0.784,0.554,0.547,0.5,0.371,0.322,0.371,0.568,0.5};
float Renderer::get_ascii_text_size(std::string_view text) {
    float w = 0;
    UTF8Parser parser(text.data(), text.size());
    while (1) {
        uint32_t c = parser.next_symbol();
        if (c == 0) break;
        if (c < 128)
            w += CHAR_WIDTHS[c];
        else
            w += 1;
    }
    return w;
}