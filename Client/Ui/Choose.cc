#include <Client/Ui/Choose.hh>

#include <iostream>

using namespace Ui;

uint8_t no_show() { return 0; }
uint8_t do_show() { return 1; }

Choose::Choose(Element *l, Element *r, std::function<uint8_t(void)> c, Style s) : 
    Element(0,0,s), first(l), second(r), chooser(c), choose_showing(0)
{
    width = l->width;
    height = l->height;
    l->refactor();
    r->refactor();
    l->should_render = do_show;
    r->should_render = no_show;
    r->showed = 1;
    r->animation = 0;
    r->animation.set(0);
    r->visible = 0;
    l->visible = 1;
}

void Choose::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    choose_showing = chooser();
    Element *rendering;
    if (choose_showing == 0) {
        first->should_render = do_show;
        second->should_render = no_show;
        if (second->visible) {
            rendering = second;
            second->render(ctx);
            first->on_render_skip(ctx);
        } else {
            rendering = first;
            first->render(ctx);
            second->on_render_skip(ctx);
        }
    } else {
        second->should_render = do_show;
        first->should_render = no_show;
        if (first->visible) {
            rendering = first;
            first->render(ctx);
            second->on_render_skip(ctx);
        } else {
            rendering = second;
            second->render(ctx);
            first->on_render_skip(ctx);
        }
    }
    width = rendering->width;
    height = rendering->height;
}

void Choose::refactor() {
    if (choose_showing == 0) first->refactor();
    else second->refactor();
}

void Choose::poll_events() {
    Element::poll_events();
    if (choose_showing == 0) first->poll_events();
    else second->poll_events();
}