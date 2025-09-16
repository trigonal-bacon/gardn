#pragma once

#include <Client/Ui/Element.hh>

#include <string>

namespace Ui {
    class TextInput : public Element {
    protected:
        std::string const name;
        std::string &ref;
        uint32_t max;
    public:
        TextInput(std::string &, float, float, uint32_t, Style = {});

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };
}