#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>

#include <cmath>

using namespace Ui;

UiLoadoutPetal *Ui::UiLoadout::petal_selected = nullptr;
UiLoadoutPetal *Ui::UiLoadout::petal_slots[2 * MAX_SLOT_COUNT] = {nullptr};
uint8_t Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
double Ui::UiLoadout::last_key_select = 0;

void Ui::forward_secondary_select() {
    Ui::UiLoadout::last_key_select = Game::timestamp;
    if (Ui::UiLoadout::selected_with_keys == MAX_SLOT_COUNT)
        Ui::UiLoadout::selected_with_keys = 0;
    else
        ++Ui::UiLoadout::selected_with_keys;
    for (uint8_t i = 0; i < MAX_SLOT_COUNT; ++i) {
        Ui::UiLoadout::selected_with_keys = Ui::UiLoadout::selected_with_keys % MAX_SLOT_COUNT;
        if (Game::cached_loadout[Game::loadout_count + Ui::UiLoadout::selected_with_keys] != PetalID::kNone) return;
        ++Ui::UiLoadout::selected_with_keys;
    }
    Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
}

void Ui::backward_secondary_select() {
    Ui::UiLoadout::last_key_select = Game::timestamp;
    if (Ui::UiLoadout::selected_with_keys == MAX_SLOT_COUNT)
        return Ui::forward_secondary_select();
    for (uint8_t i = 0; i < MAX_SLOT_COUNT; ++i) {
        Ui::UiLoadout::selected_with_keys = (Ui::UiLoadout::selected_with_keys - 1 + MAX_SLOT_COUNT) % MAX_SLOT_COUNT;
        if (Game::cached_loadout[Game::loadout_count + Ui::UiLoadout::selected_with_keys] != PetalID::kNone) return;
    }
    Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
}

static uint8_t static_to_dynamic(uint8_t static_pos) {
    if (static_pos >= Game::loadout_count) 
        return MAX_SLOT_COUNT + static_pos - Game::loadout_count;
    else
        return static_pos;
}

void Ui::ui_delete_petal(uint8_t static_pos) {
    Game::delete_petal(static_pos);
    Ui::UiLoadout::petal_slots[static_pos]->curr_pos = 2 * MAX_SLOT_COUNT;
    Game::cached_loadout[static_pos] = PetalID::kNone;
}

void Ui::ui_swap_petals(uint8_t static_pos1, uint8_t static_pos2) {
    //found static pos, now swap
    UiLoadoutPetal *a1 = Ui::UiLoadout::petal_slots[static_pos1];
    UiLoadoutPetal *a2 = Ui::UiLoadout::petal_slots[static_pos2];
    if (a1->petal_id == a2->petal_id) return;
    Game::cached_loadout[static_pos1] = a2->petal_id;
    Game::cached_loadout[static_pos2] = a1->petal_id;
    a1->static_pos = static_pos2;
    a2->static_pos = static_pos1;
    Ui::UiLoadout::petal_slots[static_pos1] = a2;
    Ui::UiLoadout::petal_slots[static_pos2] = a1;
    a1->no_change_ticks = a2->no_change_ticks = 10;
    Game::swap_petals(static_pos1, static_pos2);
}

static uint8_t dynamic_to_static(uint8_t dynamic_pos) {
    if (dynamic_pos >= MAX_SLOT_COUNT)
        dynamic_pos -= (MAX_SLOT_COUNT - Game::loadout_count);
    return dynamic_pos; 
}


static uint8_t find_viable_target() {
    for (uint8_t i = 0; i < 2 * MAX_SLOT_COUNT + 1; ++i) {
        UiLoadoutSlot *slot = Ui::UiLoadout::petal_backgrounds[i];
        if (slot != nullptr && slot->visible &&
        fabsf(Input::mouse_x - slot->screen_x) < slot->width * Ui::scale / 2 &&
        fabsf(Input::mouse_y - slot->screen_y) < slot->height * Ui::scale / 2) {
            return i;
        }
    }
    return (uint8_t)-1;
}

UiLoadoutPetal::UiLoadoutPetal(uint8_t pos) : Element(60, 60), 
    static_pos(pos), no_change_ticks(0), petal_id(PetalID::kNone), last_id(PetalID::kNone) 
{
    reload = 1;
    style.should_render = [&](){
        if (!Game::alive()) return false;
        //coincidentally also works for trashing
        if (static_pos >= MAX_SLOT_COUNT + Game::loadout_count) return false;
        if (curr_pos == 2 * MAX_SLOT_COUNT) return false;
        if (Game::alive()) {
            Entity const &player = Game::simulation.get_ent(Game::player_id);
            if (no_change_ticks == 0 || player.get_state_loadout_ids(static_pos)) {
                no_change_ticks = 0;
                petal_id = Game::cached_loadout[static_pos];
                if (petal_id != PetalID::kNone)
                    last_id = petal_id;
            } else --no_change_ticks;
        }
        if (static_pos < Game::loadout_count && Game::alive()) {
            Entity const &player = Game::simulation.get_ent(Game::player_id);
            if (player.get_state_loadout_reloads(static_pos)) {
                float old = player.loadout_reloads[static_pos] / 255.0f;
                if (old > reload) reload.set(old);
                else reload = old;
            }
        }
        else
            reload = 0;
        if (petal_id == PetalID::kNone) return false;
        return true;
    };
    style.animate = [&](Element *elt, Renderer &ctx){
        float lerp_amt = Ui::lerp_amount * 0.75;
        reload.step(lerp_amt);
        if (curr_pos != 2 * MAX_SLOT_COUNT) 
            curr_pos = static_to_dynamic(static_pos);

        UiLoadoutSlot *parent_slot = Ui::UiLoadout::petal_backgrounds[curr_pos];
        if (Ui::UiLoadout::petal_selected == this && Game::alive()) {
            style.layer = 1;
            uint8_t potential_swap = find_viable_target();
            if (potential_swap != ((uint8_t)-1) && potential_swap != static_to_dynamic(static_pos)) {
                if (BIT_AT(Input::mouse_buttons_released, Input::LeftMouse)) {
                    if (potential_swap == 2 * MAX_SLOT_COUNT)
                        ui_delete_petal(static_pos);
                    else {
                        //find static pos of both
                        potential_swap = dynamic_to_static(potential_swap);
                        ui_swap_petals(potential_swap, static_pos);
                    }
                } else {
                    UiLoadoutSlot *slot = Ui::UiLoadout::petal_backgrounds[potential_swap];
                    LERP(x, (slot->screen_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
                    LERP(y, (slot->screen_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
                    LERP(width, slot->width, lerp_amt);
                    LERP(height, slot->height, lerp_amt);
                }
            } else {
                LERP(x, (Input::mouse_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
                LERP(y, (Input::mouse_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
                LERP(width, parent_slot->width + 10, lerp_amt);
                LERP(height, parent_slot->height + 10, lerp_amt);
                ctx.rotate(sin(Game::timestamp / 150) * 0.1);
            }
            if (BIT_AT(Input::mouse_buttons_released, Input::LeftMouse))
                Ui::UiLoadout::petal_selected = nullptr;
        } else if (Ui::UiLoadout::selected_with_keys + Game::loadout_count == static_pos && Game::alive()) {
            if (!showed) lerp_amt = 1;
            LERP(x, (parent_slot->screen_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
            LERP(y, (parent_slot->screen_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
            LERP(width, parent_slot->width + 20, lerp_amt);
            LERP(height, parent_slot->height + 20, lerp_amt);
            ctx.rotate(sin(Game::timestamp / 150) * 0.1);
        } else {
            style.layer = 0;
            if (!showed) lerp_amt = 1;
            LERP(x, (parent_slot->screen_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
            LERP(y, (parent_slot->screen_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
            LERP(width, parent_slot->width, lerp_amt);
            LERP(height, parent_slot->height, lerp_amt);
        }
        if (!Game::alive())
            Ui::UiLoadout::petal_selected = nullptr;
        ctx.scale((float) animation);
    };
    Ui::UiLoadout::petal_slots[static_pos] = this;
}

void UiLoadoutPetal::on_render(Renderer &ctx) {
    ctx.scale(width / 60);
    if (static_pos < Game::loadout_count && PETAL_DATA[last_id].count != 0)
        draw_loadout_background(ctx, last_id, (float) reload);
    else
        draw_loadout_background(ctx, last_id);
}

void UiLoadoutPetal::on_render_skip(Renderer &ctx) {
    x = y = 0;
    if (Game::alive()) showed = 1;
    else showed = 0;
    curr_pos = 0;
    petal_id = last_id = PetalID::kNone;
    if (Ui::UiLoadout::petal_selected == this) 
        Ui::UiLoadout::petal_selected = nullptr;
}

void UiLoadoutPetal::on_event(uint8_t event) {
    if (Game::alive() && event == kMouseDown && Ui::UiLoadout::petal_selected == nullptr) {
        Ui::UiLoadout::petal_selected = this;
        Ui::UiLoadout::selected_with_keys = MAX_SLOT_COUNT;
    }
    if (event != kFocusLost && last_id != PetalID::kNone && Ui::UiLoadout::petal_selected == nullptr) {
        rendering_tooltip = 1;
        tooltip = Ui::UiLoadout::petal_tooltips[last_id];
    } else {
        rendering_tooltip = 0;
    }
}