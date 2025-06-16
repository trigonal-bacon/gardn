#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/InGame/Loadout.hh>
#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>
#include <Client/Ui/DynamicText.hh>
#include <Client/Assets/Assets.hh>
#include <Client/Game.hh>

#include <string>

using namespace Ui;

StatPetalSlot::StatPetalSlot(uint8_t p) : Element(50,50,{}), pos(p) {
    style.v_justify = Style::Bottom;
    style.should_render = [=](){
        return p < Game::loadout_count + MAX_SLOT_COUNT && Game::cached_loadout[p] != PetalID::kNone;
    };
}

void StatPetalSlot::refactor() {
    if (pos < Game::loadout_count) {
        width = height = 60;
    } else if (Game::cached_loadout[pos] != PetalID::kNone) {
        width = height = 45;
    } else {
        width = height = 0;
    }
}

void StatPetalSlot::on_render(Renderer &ctx) {
    uint8_t id = Game::cached_loadout[pos];
    if (id == PetalID::kNone) return;
    ctx.scale(width / 60);
    draw_loadout_background(ctx, id);
}

void StatPetalSlot::on_event(uint8_t event) {
    uint8_t id = Game::cached_loadout[pos];
    if (event != kFocusLost && id != PetalID::kNone) {
        rendering_tooltip = 1;
        tooltip = Ui::UiLoadout::petal_tooltips[id];
    } else {
        rendering_tooltip = 0;
    }
}

TitlePetalSlot::TitlePetalSlot(uint8_t p) : Element(50,50,{}), pos(p) {
    style.v_justify = Style::Bottom;
    style.should_render = [=](){
        if (!Game::simulation.ent_exists(Game::camera_id))
            return false;
        Entity const &camera = Game::simulation.get_ent(Game::camera_id);
        if (p >= loadout_slots_at_level(camera.respawn_level) + MAX_SLOT_COUNT) return false;
        return camera.inventory[p] > PetalID::kBasic;
    };
}

void TitlePetalSlot::on_render(Renderer &ctx) {
    if (!Game::simulation.ent_exists(Game::camera_id))
        return;
    Entity const &camera = Game::simulation.get_ent(Game::camera_id);
    uint8_t id = camera.inventory[pos];
    if (id == PetalID::kNone) return;
    ctx.scale(width / 60);
    draw_loadout_background(ctx, id);
}

void TitlePetalSlot::on_event(uint8_t event) {
    if (!Game::simulation.ent_exists(Game::camera_id))
        return;
    Entity const &camera = Game::simulation.get_ent(Game::camera_id);
    uint8_t id = camera.inventory[pos];
    if (event != kFocusLost && id != PetalID::kNone) {
        rendering_tooltip = 1;
        tooltip = Ui::UiLoadout::petal_tooltips[id];
    } else {
        rendering_tooltip = 0;
    }
}

Element *Ui::make_stat_screen() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Stats", { .fill = 0xffffffff, .h_justify = Style::Left }),
        new Ui::DynamicText(16, [](){
            return std::format("Score: {:.0f}", Game::score);
        }, { .fill = 0xffffffff, .h_justify = Style::Left }),
        new Ui::DynamicText(16, [](){
            return std::format("Level: {}", score_to_level(Game::score));
        }, { .fill = 0xffffffff, .h_justify = Style::Left }),
        new Ui::HContainer({
            new StatPetalSlot(0),
            new StatPetalSlot(1),
            new StatPetalSlot(2),
            new StatPetalSlot(3),
            new StatPetalSlot(4),
            new StatPetalSlot(5),
            new StatPetalSlot(6),
            new StatPetalSlot(7),
            new StatPetalSlot(8),
            new StatPetalSlot(9),
            new StatPetalSlot(10),
            new StatPetalSlot(11),
            new StatPetalSlot(12),
            new StatPetalSlot(13),
            new StatPetalSlot(14),
            new StatPetalSlot(15),
        }, 0, 10)
    }, 20, 7);
    elt->style.animate = [](Element *elt, Renderer &ctx) {
        ctx.translate(0, (1 - elt->animation) * 30 * elt->height);
    };
    elt->style.should_render = [](){
        return !Game::alive() && Game::should_render_game_ui();
    };
    elt->animation.set(0);
    elt->style.h_justify = Style::Left;
    elt->style.v_justify = Style::Bottom;
    return elt;
}