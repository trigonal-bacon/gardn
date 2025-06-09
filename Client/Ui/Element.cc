#include <Client/Ui/Element.hh>

#include <Client/Ui/Extern.hh>
#include <Client/Input.hh>

using namespace Ui;

static void default_animate(Element *elt, Renderer &ctx) {
    ctx.scale((float) elt->animation);
}

Element::Element(float w, float h, Style s) : width(w), height(h), style(s) {
    //so the animation doesnt set
    //animation.set(1);
    //animate = [&](Renderer &ctx){ ctx.scale((float) animation); };
    if (style.animate == nullptr) 
        style.animate = default_animate;
    if (style.should_render == nullptr)
        style.should_render = [](){ return 1; };
}

void Element::add_child(Element *elt) {
    children.push_back(elt);
    elt->parent = this;
    //parent/child?
}

Element *Element::set_z_to_one() {
    layer = 1;
    return this;
}

void Element::render(Renderer &ctx) {
    animation.set(style.should_render());
    if (style.no_animation) animation.step(1);
    else animation.step(Ui::lerp_amount);
    //else animation.step(1);
    float curr_animation = (float) animation;
    visible = curr_animation > 0.01;
    #ifdef DEBUG
    if (visible) {
        RenderContext context(&ctx);
        ctx.set_stroke(0x40000000);
        ctx.set_line_width(1);
        ctx.begin_path();
        ctx.round_rect(-width / 2, -height / 2, width, height, 6);
        ctx.stroke();
    }
    #endif
    if (visible) {
        style.animate(this, ctx);
        //get abs x, y;
        screen_x = ctx.context.transform_matrix[2];
        screen_y = ctx.context.transform_matrix[5];
        on_render(ctx);
        showed = 1;
        //possibly poll events?
    } else 
        on_render_skip(ctx);
    //event emitter
    if (Ui::focused == this) {
        if (BIT_AT(Input::mouse_buttons_pressed, Input::LeftMouse)) {
            focus_state = kMouseDown;
            on_event(kMouseDown);
        }
        else if (BIT_AT(Input::mouse_buttons_released, Input::LeftMouse)) {
            focus_state = kClick;
            on_event(kClick);
        }
        else if (focus_state != kMouseDown) {
            focus_state = kMouseHover;
            on_event(kMouseHover);
        }
    } else if (focus_state != kFocusLost) {
        focus_state = kFocusLost;
        on_event(kFocusLost);
    }
}

void Element::on_render(Renderer &ctx) {
    if (style.fill != 0x00000000) {
        ctx.set_fill(Renderer::HSV(style.fill, style.stroke_hsv));
        ctx.begin_path();
        ctx.round_rect(-width / 2, -height / 2, width, height, style.round_radius);
        ctx.fill();
        if (style.fill >= 0xff000000) {
            ctx.set_fill(style.fill);
            ctx.begin_path();
            ctx.rect(-width / 2 + style.line_width, -height / 2 + style.line_width, width - 2 * style.line_width, height - 2 * style.line_width);
            ctx.fill();
        }
    }
}

void Element::on_render_tooltip(Renderer &ctx) {
    tooltip_animation.set(rendering_tooltip);
    tooltip_animation.step(Ui::lerp_amount * 1.5);
    if (tooltip_animation < 0.01 && !rendering_tooltip)
        tooltip = nullptr;
    if (tooltip != nullptr) {
        ctx.reset_transform();
        if (screen_x < (tooltip->width / 2 + 10) * Ui::scale)
            ctx.translate((tooltip->width / 2 + 10) * Ui::scale, screen_y);
        else ctx.translate(screen_x, screen_y);
        ctx.scale(Ui::scale);
        ctx.translate(0, -(height + tooltip->height) / 2 - 10);
        ctx.set_global_alpha((float) tooltip_animation);
        tooltip->on_render(ctx);
    }
    for (Element *elt : children) {
        RenderContext context(&ctx);
        if (!elt->visible) continue;
        elt->on_render_tooltip(ctx);
    }
}

void Element::on_render_skip(Renderer &ctx) {
    for (Element *elt : children)
        elt->on_render_skip(ctx);
}

void Element::on_event(uint8_t event) {}

void Element::refactor() {
    for (Element *elt : children)
        if (elt->visible) elt->refactor();
}

void Element::poll_events() {
    if (std::abs(Input::mouse_x - screen_x) < width * Ui::scale / 2
    && std::abs(Input::mouse_y - screen_y) < height * Ui::scale / 2)
        Ui::focused = this;
    else if (Ui::focused == this) {
        Ui::focused = nullptr;
    }
}