#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Assets/Assets.hh>
#include <Client/Game.hh>

#include <cmath>

using namespace Ui;

UiLoadoutPetal *Ui::UiLoadout::petal_selected = nullptr;
UiLoadoutPetal *Ui::UiLoadout::petal_slots[2 * MAX_SLOT_COUNT] = {nullptr};

UiLoadoutPetal::UiLoadoutPetal(uint8_t pos) : Element(60, 60), 
    static_pos(pos), no_change_ticks(0), petal_id(PetalID::kNone), last_id(PetalID::kNone) {
    should_render = [&](){
        if (!Game::in_game()) return false;
        //coincidentally also works for trashing
        if (static_pos >= MAX_SLOT_COUNT + Game::loadout_count) return false;
        if (curr_pos == 2 * MAX_SLOT_COUNT) return false;
        if (Game::alive()) {
            Entity &player = Game::simulation.get_ent(Game::simulation.get_ent(Game::camera_id).player);
            if (no_change_ticks == 0) {
                petal_id = player.loadout_ids[static_pos];
                if (petal_id != PetalID::kNone)
                    last_id = petal_id;
            } else --no_change_ticks;
        }
        if (petal_id == PetalID::kNone) return false;
        return true;
    };
    animate = [&](Renderer &ctx){
        ctx.scale((float) animation);
        if (curr_pos != 2 * MAX_SLOT_COUNT) {
            if (static_pos >= Game::loadout_count) 
                curr_pos = MAX_SLOT_COUNT + static_pos - Game::loadout_count;
            else
                curr_pos = static_pos;
            if (curr_pos > 2 * MAX_SLOT_COUNT) curr_pos = 2 * MAX_SLOT_COUNT;
        }
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
                    if (swapping_with == 2 * MAX_SLOT_COUNT) {
                        Game::delete_petal(curr_pos);
                        curr_pos = swapping_with;
                        //delete petal packet
                    }
                    else {
                        //find static pos of both
                        if (swapping_with >= MAX_SLOT_COUNT)
                            swapping_with -= (MAX_SLOT_COUNT - Game::loadout_count);
                        //found static pos, now swap
                        UiLoadoutPetal *other = Ui::UiLoadout::petal_slots[swapping_with];
                        uint8_t tmp = static_pos;
                        static_pos = other->static_pos;
                        other->static_pos = tmp;
                        Ui::UiLoadout::petal_slots[other->static_pos] = other;
                        Ui::UiLoadout::petal_slots[static_pos] = this;
                        other->no_change_ticks = no_change_ticks = 10;
                        Game::swap_petals(static_pos, other->static_pos);
                    }
                    //else error
                }
                //null it out
                Ui::UiLoadout::petal_selected = nullptr;
            }
            else {
                float amt = 0.2;
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
                LERP(x, (wanting_x - Ui::window_width / 2) / Ui::scale, amt);
                LERP(y, (wanting_y - Ui::window_height / 2) / Ui::scale, amt);
                LERP(width, wanting_r, amt);
                LERP(height, wanting_r, amt);
                if (wanting_r == 70)
                    ctx.rotate(sin(timestamp / 150) * 0.1);
                layer = 1;
            }
        }
        else if (parent_slot != nullptr) {
            float amt = 0.2;
            if (!showed) amt = 1;
            LERP(x, (parent_slot->screen_x - Ui::window_width / 2) / Ui::scale, amt);
            LERP(y, (parent_slot->screen_y - Ui::window_height / 2) / Ui::scale, amt);
            LERP(width, parent_slot->width, amt);
            LERP(height, parent_slot->height, amt);
        }
    };
    Ui::UiLoadout::petal_slots[static_pos] = this;
}

void UiLoadoutPetal::on_render(Renderer &ctx) {
    {
        RenderContext context(&ctx);
        ctx.scale(width / 60);
        draw_loadout_background(ctx, RARITY_COLORS[PETAL_DATA[last_id].rarity]);
        ctx.translate(0, -5);
        {
            RenderContext r(&ctx);
            if (PETAL_DATA[last_id].radius > 30) ctx.scale(30 / PETAL_DATA[last_id].radius);
            draw_static_petal(last_id, ctx);
        }
        ctx.set_text_size(14);
        ctx.set_line_width(14 * 0.12);
        ctx.set_fill(0xffffffff);
        ctx.set_stroke(0xff222222);
        ctx.translate(0, 20);
        ctx.center_text_align();
        ctx.center_text_baseline();
        ctx.stroke_text(PETAL_DATA[last_id].name);
        ctx.fill_text(PETAL_DATA[last_id].name);
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
    if (event == kMouseDown) Ui::UiLoadout::petal_selected = this;
}