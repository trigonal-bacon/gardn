#include <Client/Ui/InGame/Chat.hh>

#include <Client/DOM.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>
#include <Client/Render/Renderer.hh>
#include <Client/Ui/Extern.hh>

#include <Shared/Binary.hh>
#include <Shared/Config.hh>

#include <emscripten.h>
#include <cstdlib>

using namespace Ui;

static std::string _nonce() { return "chat" + std::to_string((uint32_t)(rand()%100000)); }

ChatInput::ChatInput() : Element(150, 18, { .h_justify = Style::Center, .v_justify = Style::Center }) {
    dom_id = _nonce();
    DOM::create_text_input(dom_id.c_str(), MAX_CHAT_LENGTH);
    EM_ASM({
        Module.chatSetWhite = Module.chatSetWhite || ((id)=>{
            const elem = document.getElementById(id);
            if (!elem) return;
            elem.style.color = '#ffffff';
            elem.style.caretColor = '#ffffff';
        });
        Module.chatSetWhite(UTF8ToString($0));
    }, dom_id.c_str());
    DOM::set_placeholder(dom_id.c_str(), "Enter message...");
    style.fill = 0x80000000;
    style.line_width = 2;
    style.round_radius = 3;
    style.animate = [&](Element *elt, Renderer &ctx) {
        ctx.translate(0, Game::renderer.height/2 - 400);
    };
    style.should_render = [](){ return Game::in_game() && Game::chat_open; };
}

void ChatInput::on_render(Renderer &ctx) {
    DOM::element_show(dom_id.c_str());
    DOM::update_pos_and_dimension(dom_id.c_str(), screen_x, screen_y, width * Ui::scale, height * Ui::scale);
    buffer = DOM::retrieve_text(dom_id.c_str(), MAX_CHAT_LENGTH);
    DOM::update_text(dom_id.c_str(), buffer, MAX_CHAT_LENGTH);

    Element::on_render(ctx);

}

void ChatInput::on_render_skip(Renderer &ctx) {
    DOM::element_hide(dom_id.c_str());
}

void ChatInput::poll_events() {
    Element::poll_events();
    if (style.should_render()) {
        DOM::focus(dom_id.c_str());
    } else {
        DOM::update_text(dom_id.c_str(), "", MAX_CHAT_LENGTH);
        DOM::blur(dom_id.c_str());
    }
}

static ChatInput *g_chat_input = nullptr;

Element *Ui::make_chat_input() {
    g_chat_input = new ChatInput();
    return g_chat_input;
}

std::string Ui::get_chat_input_text() {
    if (!g_chat_input) return "";
    return DOM::retrieve_text(g_chat_input->dom_id.c_str(), MAX_CHAT_LENGTH);
}

void Ui::clear_chat_input() {
    if (!g_chat_input) return;
    DOM::update_text(g_chat_input->dom_id.c_str(), "", MAX_CHAT_LENGTH);
    DOM::blur(g_chat_input->dom_id.c_str());
}

