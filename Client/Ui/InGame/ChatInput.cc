#include <Client/Ui/InGame/Chat.hh>

#include <Client/DOM.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>

#include <Shared/Config.hh>

using namespace Ui;

ChatInput::ChatInput(std::string &r, float w, float h, uint32_t m, Style s) : TextInput(r, w, h, m, s) {}

void ChatInput::on_render(Renderer &ctx) {
    if (animation > 0.99 || Input::is_mobile) {
        if (Input::keys_held_this_tick.contains(27)) // esc
            Game::show_chat = false;
        else if (Input::toggle_chat) {
            Game::show_chat = false;
            Game::send_chat(ref);
            ref.clear();
        }
    }
    Input::toggle_chat = false;
    TextInput::on_render(ctx);
    if (style.should_render())
        DOM::element_focus(name.c_str());
}

void ChatInput::on_render_skip(Renderer &ctx) {
    if (!Game::alive()) Game::show_chat = false;
    else if (Input::toggle_chat) {
        Game::show_chat = true;
        DOM::update_text(name.c_str(), ref, max);
    }
    Input::toggle_chat = false;
    TextInput::on_render_skip(ctx);
}

Element *Ui::make_chat_input() {
    Element *elt = new Ui::ChatInput(Game::chat_text, 300, 25, MAX_CHAT_LENGTH, {
        .should_render = [](){
            return Game::show_chat && Game::alive();
        }
    });
    elt->x = 0;
    elt->y = 75;
    return elt;
}
