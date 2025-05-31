#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Game.hh>
#include <Shared/Entity.hh>

using namespace Ui;

UiLoadoutSlot *Ui::UiLoadout::petal_backgrounds[2 * MAX_SLOT_COUNT + 1] = {nullptr};

UiLoadoutSlot::UiLoadoutSlot(uint8_t pos) : Element(60, 60, { .fill = 0xfffcfcfc, .round_radius = 0 }) {
    if (pos >= MAX_SLOT_COUNT) width = height = 45;
    style.line_width = width / 12;
    style.round_radius = width / 20;
    should_render = [=](){
        //if (!Game::should_render_game_ui()) return false;
        if (pos >= MAX_SLOT_COUNT || pos < Game::loadout_count) return true;
        return false;
    };
    Ui::UiLoadout::petal_backgrounds[pos] = this;
}

UiDeleteSlot::UiDeleteSlot() : UiLoadoutSlot(2 * MAX_SLOT_COUNT) {
    style.fill = 0xffcf8888;
    delete_text_opacity.set(0);
    style.animate = [&](Element *, Renderer &) {
        delete_text_opacity.set(Ui::UiLoadout::petal_selected != nullptr);
        delete_text_opacity.step(Ui::lerp_amount);
    };
}

void UiDeleteSlot::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    if ((float) delete_text_opacity > 0.01) {
        ctx.set_global_alpha((float) delete_text_opacity);
        ctx.set_fill(0xffffffff);
        ctx.set_stroke(0xff222222);
        ctx.center_text_align();
        ctx.center_text_baseline();
        ctx.set_text_size(height / 4);
        ctx.set_line_width(height / 4 * 0.12);
        ctx.stroke_text("Delete");
        ctx.fill_text("Delete");
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
                }, 10, 25
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
            new Ui::InputFreeze()
        }, 0, 0
    );
    base->should_render = [](){
        return Game::should_render_game_ui(); //Game::in_game();
    };
    base->style.v_justify = Style::Bottom;
    base->style.animate = [](Element *elt, Renderer &ctx){};
    return base;
}