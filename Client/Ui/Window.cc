#include <Client/Ui/Window.hh>

#include <Client/Ui/Extern.hh>

#include <Client/Game.hh>

#include <cmath>

using namespace Ui;

Window::Window() : Container({}) {}

void Window::set_divider() {
    title_divider = children.size();
}

void Window::render_title_screen(Renderer &ctx) {
    RenderContext context(&ctx);
    ctx.reset_transform();
    width = Ui::window_width; height = Ui::window_height;
    ctx.translate(width / 2, height / 2);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (uint32_t i = 0; i < title_divider; ++i) {
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
}

void Window::render_game_screen(Renderer &ctx) {
    RenderContext context(&ctx);
    ctx.reset_transform();
    width = Ui::window_width; height = Ui::window_height;
    ctx.translate(width / 2, height / 2);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (uint32_t i = title_divider; i < children.size(); ++i) {
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
}

void Window::tick_render_skip(Renderer &ctx) {
    /*
    for (Element *elt : children)
        if (!elt->visible) {
            //elt->animation.set(0);
            //elt->animation.step(1);
            elt->on_render_skip(ctx);
        }
    */
    if (!Game::should_render_game_ui()) {
        for (uint32_t i = title_divider; i < children.size(); ++i) {
            children[i]->visible = 0;
            children[i]->on_render_skip(ctx);
        }
    } else if (!Game::should_render_title_ui()) {
        for (uint32_t i = 0; i < title_divider; ++i) {
            children[i]->visible = 0;
            children[i]->on_render_skip(ctx);
        }
    }
}