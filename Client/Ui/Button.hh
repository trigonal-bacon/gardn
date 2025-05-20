#pragma once

#include <Client/Ui/Element.hh>

namespace Ui {
    class Button : public Element {
    public:
        Element *label;
        void (*on_click)(uint8_t);
        Button(float, float, Element *, void (uint8_t) = [](uint8_t){}, Style = { .fill = 0xffffffff, .stroke_hsv = 0.8 });

        virtual void on_render(Renderer &) override;
        virtual void refactor() override;
        virtual void on_event(uint8_t) override;
    };
}