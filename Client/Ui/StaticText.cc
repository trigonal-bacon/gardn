#include <Client/Ui/StaticText.hh>

#include <Client/Render/Renderer.hh>

using namespace Ui;

StaticText::StaticText(float font_size, char const *str, Style s) : 
Element(Renderer::get_ascii_text_size(str) * font_size, font_size, s), text(str) {}

void StaticText::on_render(Renderer &ctx) {
    ctx.draw_text(text, { .fill = style.fill, .size = height });
}