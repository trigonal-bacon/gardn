#include <Client/Ui/ScrollContainer.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Input.hh>

#include <Helpers/Bits.hh>
#include <Helpers/Macros.hh>

using namespace Ui;

ScrollBar::ScrollBar() : Element(8, 100, { .fill = 0x40000000, .round_radius = 4 }) {
    style.animate = [&](Element *elt, Renderer &ctx){
        if (elt->style.layer && BitMath::at(Input::mouse_buttons_released, Input::LeftMouse))
            elt->style.layer = 0;
    };
}

void ScrollBar::on_event(uint8_t event) {
    if (event == kMouseDown)
        style.layer = 1;
}

ScrollContainer::ScrollContainer(Element *content, float max_height) : HContainer({content, new ScrollBar()}, 0, 10, {}) {
    height = max_height;
    lerp_scroll = 0;
}

void ScrollContainer::on_render(Renderer &ctx) {
    DEBUG_ONLY(assert(children.size() == 2));
    Element *scroll = children[1];
    Element *content = children[0];
    if (height < content->height) {
        scroll->height = height * height / content->height;
        float ratio = height == scroll->height ? 1 : (content->height - height) / (height - scroll->height);
        if (std::abs(Input::mouse_x - screen_x) < width * Ui::scale / 2
        && std::abs(Input::mouse_y - screen_y) < height * Ui::scale / 2)
            lerp_scroll += Input::wheel_delta / ratio;
        if (scroll->style.layer) lerp_scroll += (Input::mouse_y - Input::prev_mouse_y);
        if (Input::is_mobile) {
            auto iter = Input::touches.find(touch_id);
            if (iter != Input::touches.end())
                lerp_scroll -= iter->second.dy / ratio;
            else
                touch_id = (uint32_t)-1;
        }
        lerp_scroll = fclamp(lerp_scroll, 0, height - scroll->height);
        scroll->y = lerp(scroll->y, lerp_scroll, Ui::lerp_amount);
        content->y = lerp(content->y, -ratio * lerp_scroll, Ui::lerp_amount);
    } else 
        content->y = scroll->y = 0;
    RenderContext c(&ctx);
    ctx.clip_rect(0,0,width,height);
    for (Element *elt : children) {
        RenderContext context(&ctx);
        ctx.translate(elt->x, elt->y);
        ctx.translate(elt->style.h_justify * (width - elt->width) / 2, elt->style.v_justify * (height - elt->height) / 2);
        elt->render(ctx);
    }
} 

void ScrollContainer::refactor() {
    DEBUG_ONLY(assert(children.size() == 2));
    width = 0;
    for (Element *elt : children) {
        elt->style.h_justify = Style::Left;
        elt->style.v_justify = Style::Top;
        elt->refactor();
        elt->x = width;
        width += inner_pad + elt->width;
    }
    width -= inner_pad;
}

void ScrollContainer::poll_events(ScreenEvent const &event) {
    Element::poll_events(event);
    if (Ui::focused == this)
        for (Element *elt : children)
            elt->poll_events(event);
}
