#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Game.hh>

#include <cmath>

using namespace Ui;

UiLoadoutPetal *Ui::UiLoadout::petal_selected = nullptr;
UiLoadoutPetal *Ui::UiLoadout::petal_slots[2 * MAX_SLOT_COUNT] = {nullptr};
uint8_t Ui::UiLoadout::selected_with_keys = 0;

static uint8_t static_to_dynamic(uint8_t static_pos) {
    if (static_pos >= Game::loadout_count) 
        return MAX_SLOT_COUNT + static_pos - Game::loadout_count;
    else
        return static_pos;
}

static void ui_delete_petal(uint8_t static_pos) {
    Game::delete_petal(static_pos);
    Ui::UiLoadout::petal_slots[static_pos]->curr_pos = 2 * MAX_SLOT_COUNT;
}

static void ui_swap_petals(uint8_t static_pos1, uint8_t static_pos2) {
    //found static pos, now swap
    UiLoadoutPetal *a1 = Ui::UiLoadout::petal_slots[static_pos1];
    UiLoadoutPetal *a2 = Ui::UiLoadout::petal_slots[static_pos2];
    if (a1->petal_id == a2->petal_id) return;
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

UiLoadoutPetal::UiLoadoutPetal(uint8_t pos) : Element(60, 60), 
    static_pos(pos), no_change_ticks(0), petal_id(PetalID::kNone), last_id(PetalID::kNone) 
{
    reload = 1;
    style.should_render = [&](){
        if (!Game::should_render_game_ui()) return false;
        //coincidentally also works for trashing
        if (static_pos >= MAX_SLOT_COUNT + Game::loadout_count) return false;
        if (curr_pos == 2 * MAX_SLOT_COUNT) return false;
        if (Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::player_id);
            if (no_change_ticks == 0 || player.state_per_loadout_ids[static_pos]) {
                no_change_ticks = 0;
                petal_id = player.loadout_ids[static_pos];
                if (petal_id != PetalID::kNone)
                    last_id = petal_id;
            } else --no_change_ticks;
        }
        if (static_pos < Game::loadout_count && Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::player_id);
            if (player.state_per_loadout_reloads[static_pos]) {
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
        ctx.scale((float) animation);
        float lerp_amt = Ui::lerp_amount;
        reload.step(lerp_amt);
        if (curr_pos != 2 * MAX_SLOT_COUNT) 
            curr_pos = static_to_dynamic(static_pos);
        UiLoadoutSlot *parent_slot = Ui::UiLoadout::petal_backgrounds[curr_pos];
        layer = 0;
        if (Ui::UiLoadout::petal_selected == this && Game::alive()) {
            if (BIT_AT(Input::mouse_buttons_released, Input::LeftMouse)) {
                //check if there's a valid switch
                uint8_t swapping_with = static_pos;
                //iteration
                for (uint8_t i = 0; i < 2 * MAX_SLOT_COUNT + 1; ++i) {
                    UiLoadoutSlot *slot = Ui::UiLoadout::petal_backgrounds[i];
                    if (slot != nullptr && slot->visible &&
                    fabsf(Input::mouse_x - slot->screen_x) < slot->width * Ui::scale / 2 &&
                    fabsf(Input::mouse_y - slot->screen_y) < slot->height * Ui::scale / 2) {
                        swapping_with = i;
                        break;
                    }
                }
                if (swapping_with != static_pos) {
                    //do the swap
                    if (swapping_with == 2 * MAX_SLOT_COUNT)
                        ui_delete_petal(static_pos);
                    else {
                        //find static pos of both
                        swapping_with = dynamic_to_static(swapping_with);
                        ui_swap_petals(swapping_with, static_pos);
                    }
                    //else error
                }
                //null it out
                Ui::UiLoadout::petal_selected = nullptr;
            }
            else {
                float wanting_x = Input::mouse_x;
                float wanting_y = Input::mouse_y;
                float wanting_r = 70;
                for (uint8_t i = 0; i < 2 * MAX_SLOT_COUNT + 1; ++i) {
                    if (i == curr_pos) continue;
                    UiLoadoutSlot *slot = Ui::UiLoadout::petal_backgrounds[i];
                    if (slot != nullptr && slot->visible &&
                    fabsf(Input::mouse_x - slot->screen_x) < slot->width * Ui::scale / 2 &&
                    fabsf(Input::mouse_y - slot->screen_y) < slot->height * Ui::scale / 2) {
                        wanting_x = slot->screen_x;
                        wanting_y = slot->screen_y;
                        wanting_r = slot->width;
                        break;
                    }
                }
                LERP(x, (wanting_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
                LERP(y, (wanting_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
                LERP(width, wanting_r, lerp_amt);
                LERP(height, wanting_r, lerp_amt);
                if (wanting_r == 70)
                    ctx.rotate(sin(timestamp / 150) * 0.1);
                layer = 1;
            }
        }
        else if (parent_slot != nullptr) {
            if (!showed) lerp_amt = 1;
            LERP(x, (parent_slot->screen_x - Ui::window_width / 2) / Ui::scale, lerp_amt);
            LERP(y, (parent_slot->screen_y - Ui::window_height / 2) / Ui::scale, lerp_amt);
            LERP(width, parent_slot->width, lerp_amt);
            LERP(height, parent_slot->height, lerp_amt);
        }
        if (!Game::alive())
            Ui::UiLoadout::petal_selected = nullptr;
    };
    Ui::UiLoadout::petal_slots[static_pos] = this;
}

void UiLoadoutPetal::on_render(Renderer &ctx) {
    {
        RenderContext context(&ctx);
        ctx.scale(width / 60);
        draw_loadout_background(ctx, RARITY_COLORS[PETAL_DATA[last_id].rarity]);
        if (static_pos < Game::loadout_count && PETAL_DATA[last_id].count != 0) {
            float rld =  1 - (float) reload;
            {
                rld = rld * rld * rld * (rld * (6.0f * rld - 15.0f) + 10.0f);
                RenderContext context(&ctx);
                ctx.set_fill(0x40000000);
                ctx.clip();
                ctx.begin_path();
                ctx.move_to(0,0);
                ctx.partial_arc(0, 0, 90, -M_PI / 2 - rld * M_PI * 10, -M_PI / 2 - rld * M_PI * 8, 0);
                ctx.fill();
            }
        }
        ctx.translate(0, -5);
        {
            RenderContext r(&ctx);
            if (PETAL_DATA[last_id].radius > 30) ctx.scale(30 / PETAL_DATA[last_id].radius);
            draw_static_petal(last_id, ctx);
        }
        float text_width = 14 * Renderer::get_ascii_text_size(PETAL_DATA[last_id].name);
        if (text_width < 50) text_width = 14;
        else text_width = 14 * 50 / text_width;
        ctx.translate(0, 20);
        ctx.draw_text(PETAL_DATA[last_id].name, { .size = text_width });
    }
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
        Ui::UiLoadout::selected_with_keys = 0;
    }
    if (event == kMouseHover && last_id != PetalID::kNone && Ui::UiLoadout::petal_selected != this) {
        rendering_tooltip = 1;
        tooltip = Ui::UiLoadout::petal_tooltips[last_id];
    } else {
        rendering_tooltip = 0;
    }
}