#include <Client/Ui/TextInput.hh>

#include <Client/Ui/Extern.hh>
#include <Client/DOM.hh>

using namespace Ui;

TextInput::TextInput(char const *name, float w, float h, uint32_t max, Style s) : Element(w, h, s), name(name) {
    style.fill = 0xffeeeeee;
    style.stroke_hsv = 0;
    DOM::create_text_input(name, 16);
}

void TextInput::on_render(Renderer &ctx) {
    DOM::element_show(name);
    DOM::update_pos_and_dimension(name, screen_x, screen_y, width * Ui::scale, height * Ui::scale);
    Element::on_render(ctx);
}

void TextInput::on_render_skip(Renderer &ctx) {
    DOM::element_hide(name);
}