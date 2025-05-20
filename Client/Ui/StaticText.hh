#pragma once

#include <Client/Ui/Element.hh>

namespace Ui {
    class StaticText : public Element {
    public:
        char const *text;

        StaticText(float, char const *, Style = { .fill = 0xffffffff });

        virtual void on_render(Renderer &) override;
    };
}