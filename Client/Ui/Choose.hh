#pragma once

#include <Client/Ui/Element.hh>

namespace Ui {
    class Choose : public Element {
    public:
        Element *first;
        Element *second;
        std::function<uint8_t(void)> chooser;
        uint8_t choose_showing;
        Choose(Element *, Element *, std::function<uint8_t(void)>, Style = {});

        virtual void on_render(Renderer &) override;
        virtual void refactor() override;
        virtual void poll_events() override;
    };
}