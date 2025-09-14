#pragma once

#include <Client/Ui/Element.hh>

namespace Ui {
    class LevelBar final : public Element {
    public:
        LerpFloat progress;
        uint32_t level;
        LevelBar();
        virtual void on_render(Renderer &) override;
    };

    class LeaderboardSlot final : public Element {
    public:
        uint8_t pos;
        LerpFloat ratio;
        LeaderboardSlot(uint8_t);

        virtual void on_render(Renderer &) override;
    };

    class Minimap final : public Element {
    public:
        Minimap(float);
        virtual void on_render(Renderer &) override;
    };

    class OverlevelTimer final : public Element {
    public:
        OverlevelTimer(float);
        virtual void on_render(Renderer &) override;
    };

    class MobileJoyStick final : public Element {
        float joystick_x;
        float joystick_y;
        float joystick_radius;
        uint32_t persistent_touch_id;
        uint8_t is_pressed;
    public:
        MobileJoyStick(float, float, float);
        virtual void on_render(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };

    Element *make_leaderboard();
    Element *make_level_bar();
    Element *make_minimap();
    Element *make_overlevel_indicator();
    Element *make_mobile_attack_button();
    Element *make_mobile_defend_button();
    Element *make_mobile_swap_all_button();
    Element *make_mobile_chat_button();
    Element *make_mobile_fullscreen_button();
    Element *make_mobile_joystick();
}