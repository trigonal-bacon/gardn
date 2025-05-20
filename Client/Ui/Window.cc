#include <Client/Ui/Window.hh>

#include <Client/Ui/Extern.hh>

#include <iostream>

using namespace Ui;

Window::Window() : Container({}) {}

void Window::render(Renderer &ctx) {
    width = Ui::window_width; height = Ui::window_height;
    ctx.translate(width / 2, height / 2);
    on_render(ctx);
}

void Window::on_render(Renderer &ctx) {
    for (Element *elt : children) {
        RenderContext context(&ctx);
        ctx.translate(elt->h_justify * width / 2, elt->v_justify * height / 2);
        ctx.scale(Ui::scale);
        ctx.translate(-elt->h_justify * elt->width / 2, -elt->v_justify * elt->height / 2);
        ctx.translate(elt->x, elt->y);
        elt->render(ctx);
    }
}