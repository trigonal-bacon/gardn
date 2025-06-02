#pragma once

#include <Client/Ui/Element.hh>

#include <string>

namespace Ui {
    class TextInput : public Element {
    public:
        char const *name;
        TextInput(char const *, float, float, uint32_t, Style = {});

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
    };
}