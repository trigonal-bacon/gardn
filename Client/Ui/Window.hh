#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
    class Window final : public Container {
    public:
        Window();

        void render(Renderer &);
        virtual void on_render(Renderer &) override;

        virtual void poll_events(ScreenEvent const &) override;
        virtual void on_event(uint8_t) override;
    };
}