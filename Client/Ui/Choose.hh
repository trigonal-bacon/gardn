#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
    class Choose : public Container {
        std::function<uint8_t(void)> const chooser;
        uint8_t choose_showing;
    public:
        Choose(Element *, Element *, std::function<uint8_t(void)> const &, Style = {});

        virtual void on_render(Renderer &) override;
        virtual void refactor() override;
        virtual void poll_events(ScreenEvent const &) override;
    };
}