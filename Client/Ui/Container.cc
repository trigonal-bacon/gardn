#include <Client/Ui/Container.hh>

#include <cmath>

using namespace Ui;

Container::Container(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Element(0, 0, s), outer_pad(opad), inner_pad(ipad), children(elts)
{}

void Container::add_child(Element *elt) {
    children.push_back(elt);
    //parent/child?
}

void Container::on_render(Renderer &ctx) {
    if (style.fill != 0x00000000) {
        ctx.set_fill(style.fill);
        ctx.set_stroke(Renderer::HSV(style.fill, style.stroke_hsv));
        ctx.set_line_width(style.line_width);
        ctx.round_line_cap();
        ctx.round_line_join();
        ctx.begin_path();
        ctx.round_rect(-width / 2, -height / 2, width, height, style.round_radius);
        ctx.fill();
        if (style.fill >= 0xff000000) ctx.stroke();
    }
    for (Element *elt : children) {
        RenderContext context(&ctx);
        ctx.translate(elt->x, elt->y);
        ctx.translate(elt->h_justify * (width - elt->width) / 2, elt->v_justify * (height - elt->height) / 2);
        elt->render(ctx);
    }
}

void Container::refactor() {
    for (Element *elt : children)
        elt->refactor();
}

void Container::poll_events() {
    Element::poll_events();
    for (Element *elt : children)
        elt->poll_events();
}

HContainer::HContainer(std::initializer_list<Element *> elts, float opad, float ipad, Style s) :
    Container(elts, opad, ipad, s) 
{
    for (Element *elt : children)
        elt->h_justify = Left;
}

void HContainer::refactor() {
    float x = outer_pad;
    float y = 0;
    for (Element *elt : children) {
        //elt->h_justify = -1;
        elt->refactor();
        if (!elt->visible) continue;
        //if (!elt->rendering || elt->detached) continue;
        //Layout l = elt->get_layout();
        elt->x = x;
        elt->y = -elt->v_justify * outer_pad;
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
        elt->v_justify = Top;
}

void VContainer::refactor() {
    float x = 0;
    float y = outer_pad;
    for (Element *elt : children) {
        //elt->v_justify = -1;
        elt->refactor();
        if (!elt->visible) continue;
        //if (!elt->rendering || elt->detached) continue;
        //Layout l = elt->get_layout();
        elt->x = -elt->h_justify * outer_pad;
        elt->y = y;
        x = fmax(x, elt->width);
        y += elt->height + inner_pad;
    }
    x += 2 * outer_pad;
    y += outer_pad - inner_pad;
    width = x;
    height = y;
}
