#include <Client/Ui/Window.hh>

#include <Client/Ui/Extern.hh>

#include <cmath>

using namespace Ui;

Window::Window() : Container({}) {}

void Window::render(Renderer &ctx) {
    Ui::lerp_amount = 1 - pow(1 - 0.3, Ui::dt * 60 / 1000);
    width = Ui::window_width; height = Ui::window_height;
    ctx.translate(width / 2, height / 2);
    on_render(ctx);
}

void Window::on_render(Renderer &ctx) {
    for (Element *elt : children) {
        if (elt->layer != 0) continue;
        RenderContext context(&ctx);
        ctx.translate(elt->h_justify * width / 2, elt->v_justify * height / 2);
        ctx.scale(Ui::scale);
        ctx.translate(-elt->h_justify * elt->width / 2, -elt->v_justify * elt->height / 2);
        ctx.translate(elt->x, elt->y);
        elt->render(ctx);
    }
    for (Element *elt : children) {
        if (elt->layer != 1) continue;
        RenderContext context(&ctx);
        ctx.translate(elt->h_justify * width / 2, elt->v_justify * height / 2);
        ctx.scale(Ui::scale);
        ctx.translate(-elt->h_justify * elt->width / 2, -elt->v_justify * elt->height / 2);
        ctx.translate(elt->x, elt->y);
        elt->render(ctx);
    }
}