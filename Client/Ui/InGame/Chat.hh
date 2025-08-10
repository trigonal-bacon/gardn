#pragma once

#include <Client/Ui/Element.hh>
#include <string>

namespace Ui {
    class ChatInput final : public Element {
    public:
        std::string buffer;
        std::string dom_id;
        ChatInput();
        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void poll_events() override;
    };

    Element *make_chat_input();
    std::string get_chat_input_text();
    void clear_chat_input();
}
