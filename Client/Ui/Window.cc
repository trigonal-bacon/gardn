#include <Client/Ui/Window.hh>

#include <Client/Ui/Extern.hh>

#include <Client/Game.hh>
#include <Client/Input.hh>

#include <cmath>

using namespace Ui;

Window::Window() : Container({}) {}

void Window::render(Renderer &ctx) {
    width = Ui::window_width;
    height = Ui::window_height;
    refactor();
    RenderContext context(&ctx);
    ctx.reset_transform();
    ctx.translate(width / 2, height / 2);
    Element::render(ctx);
}

void Window::on_render(Renderer &ctx) {
    RenderContext context(&ctx);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (uint32_t i = 0; i < children.size(); ++i) {
            Element *elt = children[i];
            if (elt->style.layer != layer) continue;
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

void Window::poll_events(ScreenEvent const &event) {
    if (style.no_polling) return;
    Element::poll_events(event);
    for (Element *elt : children)
        if (elt->visible) elt->poll_events(event);
}

void Window::on_event(uint8_t event) {
    if (event == Ui::kMouseDown)
        Ui::panel_open = Ui::Panel::kNone;
}