#include <Client/Ui/TextInput.hh>

#include <Client/Ui/Extern.hh>
#include <Client/DOM.hh>

#include <iostream>
using namespace Ui;

TextInput::TextInput(std::string &r, char const *name, float w, float h, uint32_t m, Style s) : Element(w, h, s), name(name), ref(r), max(m) {
    style.fill = 0xffeeeeee;
    style.stroke_hsv = 0;
    DOM::create_text_input(name, max);
    DOM::update_text(name, r, max);
}

void TextInput::on_render(Renderer &ctx) {
    DOM::element_show(name);
    DOM::update_pos_and_dimension(name, screen_x, screen_y, width * Ui::scale, height * Ui::scale);
    ref = DOM::retrieve_text(name, max);
    DOM::update_text(name, ref, max);
    Element::on_render(ctx);
}

void TextInput::on_render_skip(Renderer &ctx) {
    DOM::element_hide(name);
}