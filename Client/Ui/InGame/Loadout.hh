#pragma once

#include <Client/Ui/Choose.hh>
#include <Client/Ui/Element.hh>

#include <Shared/StaticData.hh>

namespace Ui {
    class UiLoadoutSlot : public Element {
    public:
        uint8_t position;
        UiLoadoutSlot(uint8_t);
    };

    class UiDeleteSlot final : public UiLoadoutSlot {
    public:
        UiDeleteSlot();
    };

    class InputFreeze final : public Choose {
    public:
        float render_width;
        float render_height;
        InputFreeze();
        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void on_event(uint8_t) override;
        virtual void poll_events() override;
    };

    class UiLoadoutPetal final : public Element {
    public:
        LerpFloat reload;
        uint8_t static_pos;
        uint8_t curr_pos;
        uint8_t no_change_ticks;
        PetalID::T petal_id;
        PetalID::T last_id;
        UiLoadoutPetal(uint8_t);

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };

    namespace UiLoadout {
        extern UiLoadoutPetal *petal_selected;
        extern UiLoadoutPetal *petal_slots[2 * MAX_SLOT_COUNT];
        extern UiLoadoutSlot *petal_backgrounds[2 * MAX_SLOT_COUNT + 1];
        extern uint8_t selected_with_keys;
    };

    Element *make_loadout_backgrounds();
}