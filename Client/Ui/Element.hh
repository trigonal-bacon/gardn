#pragma once

#include <Client/Render/Renderer.hh>
#include <Shared/Helpers.hh>

#include <functional>

namespace Ui {
    struct Style {
        uint32_t fill = 0x00000000;
        float stroke_hsv = 0.8;
        float line_width = 6;
        float round_radius = 0;
    };

    enum UiEvent {
        kFocusLost,
        kMouseDown,
        kClick,
        kMouseUp,
        kMouseMove,
        kMouseHover
    };

    class Element {
    public:
        enum {
            Top = -1,
            Middle = 0,
            Bottom = 1,
            Left = -1,
            Center = 0,
            Right = 1
        };

        Ui::Style style;
        float width = 0;
        float height = 0;
        float x = 0;
        float y = 0;
        float screen_x = 0;
        float screen_y = 0;
        LerpFloat animation;
        int8_t h_justify = Center;
        int8_t v_justify = Middle;
        uint8_t visible = 1;
        uint8_t showed = 0;
        uint8_t focus_state = 0;

        Element(float = 0, float = 0, Style = {});
        void render(Renderer &);
        virtual void animate(Renderer &);
        virtual void on_render(Renderer &);
        virtual void on_render_skip(Renderer &);
        virtual void on_event(uint8_t);
        std::function<bool(void)> should_render = [](){ return true; };
        virtual void refactor();
        virtual void poll_events();
    };
}