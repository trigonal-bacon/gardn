#pragma once

#include <Client/Render/Renderer.hh>
#include <Shared/Helpers.hh>

namespace Ui {
    class Element {
        float width = 0;
        float height = 0;
        LerpFloat animation;
        int h_justify;
        int v_justify;
        int visible = 1;
    public:
        enum {
            Top = -1,
            Middle = 0,
            Bottom = 1,
            Left = -1,
            Center = 0,
            Right = 1
        };
        Element(float, float);
        virtual void animate(Renderer &);
        void render(Renderer &);
        virtual void on_render();
        virtual void on_event();
        virtual bool should_show();
    };
}