#include <Client/Ui/TextInput.hh>

#include <Client/Ui/Extern.hh>
#include <Client/DOM.hh>

using namespace Ui;

static std::string _gen_nonce() {
    return "fjs" + std::to_string(static_cast<uint32_t>(frand() * 49263 + 672));
}

TextInput::TextInput(std::string &r, float w, float h, uint32_t m, Style s) : 
    Element(w, h, s), name(_gen_nonce()), ref(r), max(m) {
    style.fill = 0xffeeeeee;
    style.stroke_hsv = 0;
    DOM::create_text_input(name.c_str(), max);
    DOM::update_text(name.c_str(), r, max);
}

void TextInput::on_render(Renderer &ctx) {
    DOM::element_show(name.c_str());
    DOM::update_pos_and_dimension(name.c_str(), screen_x, screen_y, width * Ui::scale, height * Ui::scale);
    ref = DOM::retrieve_text(name.c_str(), max);
    DOM::update_text(name.c_str(), ref, max);
    Element::on_render(ctx);
}

void TextInput::on_render_skip(Renderer &ctx) {
    DOM::element_hide(name.c_str());
}