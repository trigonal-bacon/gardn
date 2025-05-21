#pragma once

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

    class UiLoadoutPetal final : public Element {
    public:
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
    };

    Element *make_loadout_backgrounds();
}