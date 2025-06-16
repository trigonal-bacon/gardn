#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Game.hh>
#include <Shared/Entity.hh>

using namespace Ui;

UiLoadoutSlot *Ui::UiLoadout::petal_backgrounds[2 * MAX_SLOT_COUNT + 1] = {nullptr};

UiLoadoutSlot::UiLoadoutSlot(uint8_t pos) : Element(70, 70, { .fill = 0xffeeeeee, .round_radius = 0 }) {
    if (pos >= MAX_SLOT_COUNT) width = height = 50;
    style.line_width = width / 12;
    style.round_radius = width / 20;
    style.should_render = [=](){
        if (pos >= MAX_SLOT_COUNT || pos < Game::loadout_count) return true;
        return false;
    };
    position = pos;
    Ui::UiLoadout::petal_backgrounds[pos] = this;
}

void UiLoadoutSlot::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    if (Game::slot_indicator_opacity < 0.01) return;
    if (position < Game::loadout_count) {
        ctx.set_global_alpha(Game::slot_indicator_opacity);
        ctx.translate(0, -height / 2 - 15);
        char str[4] = {'[', static_cast<char>('1' + position), ']', '\0'};
        ctx.draw_text(&str[0], { .size = 16 });
    } else if (position == 2 * MAX_SLOT_COUNT) {
        ctx.set_global_alpha(Game::slot_indicator_opacity);
        ctx.translate(0, -height / 2 - 15);
        ctx.draw_text("[T]", { .size = 16 });
        ctx.translate(0, height / 2 + 15);
    }
}

UiDeleteSlot::UiDeleteSlot() : UiLoadoutSlot(2 * MAX_SLOT_COUNT) {
    style.fill = 0xffcf8888;
    delete_text_opacity.set(0);
    style.animate = [&](Element *, Renderer &) {
        delete_text_opacity.set(Ui::UiLoadout::petal_selected != nullptr || Ui::UiLoadout::selected_with_keys != MAX_SLOT_COUNT);
        delete_text_opacity.step(Ui::lerp_amount);
    };
}

void UiDeleteSlot::on_render(Renderer &ctx) {
    UiLoadoutSlot::on_render(ctx);
    if ((float) delete_text_opacity > 0.01) {
        ctx.set_global_alpha((float) delete_text_opacity);
        ctx.draw_text("Delete", { .size = height / 4});
    }
}

Element *Ui::make_loadout_backgrounds() {
    Element *base = new Ui::VContainer(
        {
            (new Ui::HContainer(
                {
                    new Ui::UiLoadoutSlot(0),
                    new Ui::UiLoadoutSlot(1),
                    new Ui::UiLoadoutSlot(2),
                    new Ui::UiLoadoutSlot(3),
                    new Ui::UiLoadoutSlot(4),
                    new Ui::UiLoadoutSlot(5),
                    new Ui::UiLoadoutSlot(6),
                    new Ui::UiLoadoutSlot(7),
                }, 5, 20
            ))->set_z_to_one(),
            (new Ui::HContainer(
                {
                    new Ui::UiLoadoutSlot(8),
                    new Ui::UiLoadoutSlot(9),
                    new Ui::UiLoadoutSlot(10),
                    new Ui::UiLoadoutSlot(11),
                    new Ui::UiLoadoutSlot(12),
                    new Ui::UiLoadoutSlot(13),
                    new Ui::UiLoadoutSlot(14),
                    new Ui::UiLoadoutSlot(15),
                    new Ui::UiDeleteSlot()
                }, 10, 15
            ))->set_z_to_one(),
            new Ui::InputFreeze(),
            new Ui::Element(0,34,{ .should_render = [](){ return Input::keyboard_movement; }})
        }, 0, 0, { .should_render = [](){ return Game::alive(); } }
    );
    base->style.v_justify = Style::Bottom;
    base->style.animate = [](Element *elt, Renderer &ctx){
        ctx.translate(0, (1 - elt->animation) * elt->height * 2);
    };
    return base;
}