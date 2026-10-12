#include <Client/Ui/Container.hh>

#include <Client/Ui/Extern.hh>

#include <cmath>

using namespace Ui;

Container::Container(std::vector<std::unique_ptr<Element>> elts, float w, float h, Style s) :
    Element(w, h, s)
{
    for (auto &elt : elts)
        add_child(std::move(elt));
}

void Container::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    for (uint32_t layer = 0; layer < 2; ++layer) {
        for (auto &elt : children) {
            if (elt->style.layer != layer) continue;
            RenderContext context(&ctx);
            ctx.translate(elt->x, elt->y);
            ctx.translate(elt->style.h_justify * (width - elt->width) / 2, elt->style.v_justify * (height - elt->height) / 2);
            elt->render(ctx);
        }
    }
}

void Container::poll_events(ScreenEvent const &event) {
    if (style.no_polling) return;
    Element::poll_events(event);
    if (Ui::focused != this)
        return;
    for (auto &elt : children)
        if (elt->visible) elt->poll_events(event);
}

HContainer::HContainer(std::vector<std::unique_ptr<Element>> elts, float opad, float ipad, Style s) :
    Container(std::move(elts), 0, 0, s), outer_pad(opad), inner_pad(ipad)
{
    for (auto &elt : children)
        elt->style.h_justify = Style::Left;
    refactor();
}

void HContainer::refactor() {
    float x = outer_pad;
    float y = 0;
    for (auto &elt : children) {
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

VContainer::VContainer(std::vector<std::unique_ptr<Element>> elts, float opad, float ipad, Style s) :
    Container(std::move(elts), 0, 0, s), outer_pad(opad), inner_pad(ipad)
{
    for (auto &elt : children)
        elt->style.v_justify = Style::Top;
    refactor();
}

void VContainer::refactor() {
    float x = 0;
    float y = outer_pad;
    for (auto &elt : children) {
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

HFlexContainer::HFlexContainer(std::unique_ptr<Element> l, std::unique_ptr<Element> r, float opad, float ipad, Style s) : 
    Container(make_children(std::move(l), std::move(r)),0,0,s), outer_pad(opad), inner_pad(ipad) 
{
    //style.h_flex = 1;
    children[0]->style.h_justify = Style::Left;
    children[1]->style.h_justify = Style::Right;
    refactor();
}

void HFlexContainer::refactor() {
    Container::refactor();
    width = children[0]->width + inner_pad + children[1]->width;
    height = fmax(children[0]->height, children[1]->height);
}

void HFlexContainer::on_render(Renderer &ctx) {
    if (auto p = parent.get(); p)
        width = fmax(width, p->width - 2 * outer_pad);
    Container::on_render(ctx);
}