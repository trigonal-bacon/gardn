#include <Client/Ui/Window.hh>

#include <Client/Ui/Extern.hh>

#include <Client/Game.hh>

#include <cmath>

using namespace Ui;

Window::Window() : Container({}) {}

void Window::render(Renderer &ctx) {
    refactor();
    RenderContext context(&ctx);
    ctx.reset_transform();
    ctx.translate(width / 2, height / 2);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (uint32_t i = 0; i < children.size(); ++i) {
            Element *elt = children[i];
            if (elt->layer != layer) continue;
            RenderContext context(&ctx);
            ctx.translate(elt->style.h_justify * width / 2, elt->style.v_justify * height / 2);
            ctx.scale(Ui::scale);
            ctx.translate(-elt->style.h_justify * elt->width / 2, -elt->style.v_justify * elt->height / 2);
            ctx.translate(elt->x, elt->y);
            elt->render(ctx);
        }
    }
    on_render_tooltip(ctx);
}

void Window::refactor() {
    width = Ui::window_width;
    height = Ui::window_height;
    Container::refactor();
}