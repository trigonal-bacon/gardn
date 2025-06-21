#pragma once

#include <Client/Ui/Container.hh>
#include <Client/Ui/Element.hh>

#include <string>

namespace Ui {
    class StaticText : public Element {
        std::string const text;
    public:
        StaticText(float, std::string const &, Style = { .fill = 0xffffffff });
        //StaticText(float, char const *, Style = { .fill = 0xffffffff });

        virtual void on_render(Renderer &) override;
    };

    class StaticParagraph : public VContainer {
    public:
        StaticParagraph(float, float, std::string, Style = {});
    };
}