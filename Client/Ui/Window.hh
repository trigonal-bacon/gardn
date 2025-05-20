#pragma once

#include <Client/Ui/Container.hh>

namespace Ui {
    class Window final : public Container {
    public:
        Window();
        void render(Renderer &);

        virtual void on_render(Renderer &) override;
    };
}