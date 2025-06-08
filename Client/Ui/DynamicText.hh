#pragma once

#include <Client/Ui/Element.hh>

#include <functional>
#include <string>

namespace Ui {
    class DynamicText : public Element {
        std::function<std::string(void)> generator;
        std::string text;
    public:
        DynamicText(float, std::function<std::string(void)>, Style = { .fill = 0xffffffff });

        virtual void on_render(Renderer &) override;

        virtual void refactor() override;
    };
}