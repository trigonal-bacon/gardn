#include <Client/Ui/Container.hh>

#include <cmath>
#include <iostream>

using namespace Ui;

Container::Container(std::initializer_list<Element *> elts, float w, float h, Style s) :
    Element(w, h, s)
{
    children = elts;
    for (Element *elt : children) elt->parent = this;
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

void Container::poll_events() {
    Element::poll_events();
    for (Element *elt : children)
        if (elt->visible) elt->poll_events();
}

HContainer::HContainer(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Container(elts, 0, 0, s), outer_pad(opad), inner_pad(ipad)
{
    for (Element *elt : children)
        elt->style.h_justify = Style::Left;
    refactor();
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
    Container(elts, 0, 0, s), outer_pad(opad), inner_pad(ipad)
{
    for (Element *elt : children)
        elt->style.v_justify = Style::Top;
    refactor();
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
    for (Element *elt : children)
        if (elt->visible && elt->style.h_flex) elt->width = fmax(width - 2 * outer_pad, elt->width);
}

HFlexContainer::HFlexContainer(Element *l, Element *r, float pad, Style s) : 
    Container({l,r},0,0,s), inner_pad(pad) 
{
    style.h_flex = 1;
    l->style.h_justify = Style::Left;
    r->style.h_justify = Style::Right;
    refactor();
}

void HFlexContainer::refactor() {
    width = children[0]->width + inner_pad + children[1]->width;
    height = fmax(children[0]->height, children[1]->height);
}