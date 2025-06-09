#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
    class Window final : public Container {
        uint32_t title_divider = 0;
        uint32_t game_divider = 0;
    public:
        Window();

        void set_title_divider();
        void set_game_divider();

        void render_title_screen(Renderer &);

        void render_game_screen(Renderer &);

        void render_others(Renderer &);

        void tick_render_skip(Renderer &);

        void refactor();
    };
}