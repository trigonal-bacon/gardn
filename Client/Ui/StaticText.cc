#include <Client/Ui/StaticText.hh>

#include <Client/Render/Renderer.hh>

using namespace Ui;

StaticText::StaticText(float font_size, char const *str, Style s) : 
Element(Renderer::get_ascii_text_size(str) * font_size, font_size, s), text(str) {}

void StaticText::on_render(Renderer &ctx) {
    ctx.set_fill(style.fill);
    ctx.set_stroke(0xff222222);
    ctx.center_text_align();
    ctx.center_text_baseline();
    ctx.set_line_width(height * 0.12);
    ctx.set_text_size(height);
    ctx.stroke_text(text);
    ctx.fill_text(text);
}