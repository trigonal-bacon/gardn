#include <Client/Ui/Choose.hh>

#include <iostream>

using namespace Ui;

uint8_t no_show() { return 0; }
uint8_t do_show() { return 1; }

Choose::Choose(Element *l, Element *r, std::function<uint8_t(void)> const &c, Style s) : 
    Container({l, r},0,0,s), chooser(std::move(c)), choose_showing(0)
{
    width = l->width;
    height = l->height;
    l->refactor();
    r->refactor();
    l->style.should_render = do_show;
    r->style.should_render = no_show;
    r->showed = 1;
    r->animation = 0;
    r->animation.set(0);
    r->visible = 0;
    l->visible = 1;
}

void Choose::on_render(Renderer &ctx) {
    Element *first = children[0];
    Element *second = children[1];
    Element::on_render(ctx);
    choose_showing = chooser();
    Element *rendering;
    if (choose_showing == 0) {
        first->style.should_render = do_show;
        second->style.should_render = no_show;
        if (second->visible) {
            rendering = second;
            second->render(ctx);
            first->on_render_skip(ctx);
        }
        if (!second->visible) {
            rendering = first;
            first->render(ctx);
            second->on_render_skip(ctx);
        }
    } else {
        second->style.should_render = do_show;
        first->style.should_render = no_show;
        if (first->visible) {
            rendering = first;
            first->render(ctx);
            second->on_render_skip(ctx);
        }
        if (!first->visible) {
            rendering = second;
            second->render(ctx);
            first->on_render_skip(ctx);
        }
    }
    width = rendering->width;
    height = rendering->height;
}

void Choose::refactor() {
    children[choose_showing]->refactor();
}

void Choose::poll_events() {
    Element::poll_events();
    children[choose_showing]->poll_events();
}