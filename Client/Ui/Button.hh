#pragma once

#include <Client/Ui/Element.hh>

namespace Ui {
    class Button : public Element {
        void (*on_click)(Element *, uint8_t);
    public:
        Button(float, float, Element *, void (Element *, uint8_t) = [](Element *, uint8_t){}, Style = { .fill = 0xffffffff, .stroke_hsv = 0.8 });

        virtual void on_render(Renderer &) override;
        virtual void refactor() override;
        virtual void on_event(uint8_t) override;
    };

    class ToggleButton : public Element {
        uint8_t *toggler;
        float lerp_toggle;
    public:
        ToggleButton(float, uint8_t *);

        virtual void on_render(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };
}