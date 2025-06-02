#include <Client/Ui/StaticText.hh>

#include <Client/Render/Renderer.hh>

using namespace Ui;

StaticText::StaticText(float font_size, std::string str, Style s) : 
Element(Renderer::get_ascii_text_size(str.c_str()) * font_size, font_size, s), text(std::move(str)) {}

void StaticText::on_render(Renderer &ctx) {
    ctx.draw_text(text.c_str(), { .fill = style.fill, .size = height });
}