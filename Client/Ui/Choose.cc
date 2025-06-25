#include <Client/Ui/Choose.hh>

#include <iostream>

using namespace Ui;

uint8_t no_show() { return 0; }
uint8_t do_show() { return 1; }

Choose::Choose(Element *l, Element *r, std::function<uint8_t(void)> const &c, Style s) : 
    Container({l, r},0,0,s), chooser(std::move(c)), choose_showing(c())
{
    l->showed = 1;
    r->showed = 1;
    l->refactor();
    r->refactor();
    r->animation.set(0);
    refactor();
}

void Choose::on_render(Renderer &ctx) {
    DEBUG_ONLY(assert(choose_showing < children.size());)
    Element *rendering = children[choose_showing];
    rendering->render(ctx);
    if (!rendering->visible)
        children[1 - choose_showing]->render(ctx);
}

void Choose::refactor() {
    uint8_t to_show = chooser();
    children[to_show]->style.should_render = do_show;
    children[1 - to_show]->style.should_render = no_show;
    if (!children[choose_showing]->visible || !showed)
        choose_showing = to_show;
    Element *rendering = children[choose_showing];
    rendering->refactor();
    width = rendering->width;
    height = rendering->height;
}

void Choose::poll_events() {
    Element::poll_events();
    children[choose_showing]->poll_events();
}