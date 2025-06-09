#pragma once

#include <Client/Ui/Element.hh>

#include <Shared/StaticData.hh>

namespace Ui {
    class StatPetalSlot final : public Element {
    public:
        uint8_t pos;
        StatPetalSlot(uint8_t);

        virtual void on_render(Renderer &ctx) override;
        virtual void refactor() override;

        virtual void on_event(uint8_t) override;
    };

    class TitlePetalSlot final : public Element {
    public:
        uint8_t pos;
        TitlePetalSlot(uint8_t);

        virtual void on_render(Renderer &ctx) override;

        virtual void on_event(uint8_t) override;
    };

    class GalleryPetal final : public Element {
    public:
        PetalID::T id;
        GalleryPetal(PetalID::T, float);

        virtual void on_render(Renderer &) override;
        virtual void on_event(uint8_t) override;
    };

    class PetalsCollectedIndicator final : public Element {
    public:
        PetalsCollectedIndicator(float);

        virtual void on_render(Renderer &) override;
    };

    class GalleryMob final : public Element {
    public:
        MobID::T id;
        GalleryMob(MobID::T, float);

        virtual void on_render(Renderer &) override;
    };

    Element *make_title_main_screen();
    Element *make_panel_buttons();
    Element *make_death_main_screen();
    Element *make_stat_screen();
    Element *make_settings_panel();
    Element *make_petal_gallery();
    Element *make_mob_gallery();
    Element *make_changelog();
    Element *make_debug_stats();
}