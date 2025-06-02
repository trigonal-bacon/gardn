#pragma once

#include <Client/Ui/Element.hh>

#include <string>

namespace Ui {
    class StaticText : public Element {
    public:
        std::string text;

        StaticText(float, std::string, Style = { .fill = 0xffffffff });
        //StaticText(float, char const *, Style = { .fill = 0xffffffff });

        virtual void on_render(Renderer &) override;
    };
}