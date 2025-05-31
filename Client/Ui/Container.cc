#include <Client/Ui/Container.hh>

#include <cmath>
#include <iostream>

using namespace Ui;

Container::Container(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Element(0, 0, s), outer_pad(opad), inner_pad(ipad), children(elts)
{
    for (Element *elt : children) elt->parent = this;
}

void Container::add_child(Element *elt) {
    children.push_back(elt);
    elt->parent = this;
    //parent/child?
}

void Container::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (Element *elt : children) {
            if (elt->layer != layer) continue;
            RenderContext context(&ctx);
            ctx.translate(elt->x, elt->y);
            ctx.translate(elt->style.h_justify * (width - elt->width) / 2, elt->style.v_justify * (height - elt->height) / 2);
            elt->render(ctx);
        }
    }
}

void Container::on_render_tooltip(Renderer &ctx) {
    Element::on_render_tooltip(ctx);
    for (Element *elt : children) {
        RenderContext context(&ctx);
        if (!elt->visible) continue;
        elt->on_render_tooltip(ctx);
    }
}

void Container::on_render_skip(Renderer &ctx) {
    for (Element *elt : children)
        elt->on_render_skip(ctx);
}

void Container::refactor() {
    for (Element *elt : children)
        elt->refactor();
}

void Container::poll_events() {
    Element::poll_events();
    for (Element *elt : children)
        if (elt->visible) elt->poll_events();
}

HContainer::HContainer(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Container(elts, opad, ipad, s) 
{
    for (Element *elt : children)
        elt->style.h_justify = Style::Left;
}

void HContainer::refactor() {
    float x = outer_pad;
    float y = 0;
    for (Element *elt : children) {
        elt->refactor();
        if (!elt->visible) continue;
        //if (!elt->rendering || elt->detached) continue;
        //Layout l = elt->get_layout();
        elt->x = x;
        elt->y = -elt->style.v_justify * outer_pad;
        x += elt->width + inner_pad;
        y = fmax(y, elt->height);
    }
    x += outer_pad - inner_pad;
    y += 2 * outer_pad;
    width = x;
    height = y;
}

VContainer::VContainer(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Container(elts, opad, ipad, s) 
{
    for (Element *elt : children)
        elt->style.v_justify = Style::Top;
}

void VContainer::refactor() {
    float x = 0;
    float y = outer_pad;
    for (Element *elt : children) {
        //elt->style.v_justify = -1;
        elt->refactor();
        if (!elt->visible) continue;
        //if (!elt->rendering || elt->detached) continue;
        //Layout l = elt->get_layout();
        elt->x = -elt->style.h_justify * outer_pad;
        elt->y = y;
        x = fmax(x, elt->width);
        y += elt->height + inner_pad;
    }
    x += 2 * outer_pad;
    y += outer_pad - inner_pad;
    width = x;
    height = y;
}
