#pragma once

#include <Client/Ui/TextInput.hh>

namespace Ui {
    class ChatInput final : public TextInput {
    public:
        ChatInput(std::string &, float, float, uint32_t, Style = {});

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
    };

    Element *make_chat_input();
}
