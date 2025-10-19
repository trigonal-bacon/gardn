#pragma once

#include <Client/Ui/Choose.hh>
#include <Client/Ui/Element.hh>

#include <Client/StaticData.hh>

namespace Ui {
    class UiLoadoutSlot : public Element {
    public:
        uint8_t position;
        UiLoadoutSlot(uint8_t);

        virtual void on_render(Renderer &) override;
    };

    class UiDeleteSlot final : public UiLoadoutSlot {
    public:
        UiDeleteSlot();
        LerpFloat delete_text_opacity;
        virtual void on_render(Renderer &) override;
    };

    class InputFreeze final : public Container {
    public:
        float render_width;
        float render_height;
        InputFreeze();
        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void on_event(uint8_t) override;
        virtual void poll_events(ScreenEvent const &) override;
    };

    class UiLoadoutPetal final : public Element {
    public:
        LerpFloat reload;
        uint32_t persistent_touch_id;
        uint8_t static_pos;
        uint8_t curr_pos;
        uint8_t no_change_ticks;
        uint8_t selected;
        PetalID::T petal_id;
        PetalID::T last_id;
        UiLoadoutPetal(uint8_t);

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };

    namespace UiLoadout {
        extern UiLoadoutPetal *petal_slots[2 * MAX_SLOT_COUNT];
        extern UiLoadoutSlot *petal_backgrounds[2 * MAX_SLOT_COUNT + 1];
        extern Element *petal_tooltips[PetalID::kNumPetals];
        extern uint8_t selected_with_keys;
        extern double last_key_select;
        extern uint32_t num_petals_selected;
    };

    Element *make_loadout_backgrounds();
    void make_petal_tooltips();

    void forward_secondary_select();
    void backward_secondary_select();
    void ui_delete_petal(uint8_t);
    void ui_swap_petals(uint8_t, uint8_t);
}