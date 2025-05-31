#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
    class Window final : public Container {
    public:
        uint32_t title_divider = 0;
        Window();

        //void render(Renderer &);

        void render_title_screen(Renderer &);

        void render_game_screen(Renderer &);

        void tick_render_skip(Renderer &);

        //virtual void on_render(Renderer &) override;
    };
}