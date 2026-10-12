#pragma once

#include <Client/Ui/Element.hh>

#include <Shared/StaticData.hh>

namespace Ui {
    class StatPetalSlot final : public Element {
    public:
        uint8_t pos;
        StatPetalSlot(uint8_t);

        virtual void on_render(Renderer &) override;
        virtual void refactor() override;

        virtual void on_event(uint8_t) override;
    };

    class DeadFlowerIcon final : public Element {
    public:
        DeadFlowerIcon(float);

        virtual void on_render(Renderer &) override;
    };

    class TitlePetalSlot final : public Element {
    public:
        uint8_t pos;
        TitlePetalSlot(uint8_t);

        virtual void on_render(Renderer &) override;

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

    std::unique_ptr<Element> make_title_input_box();
    std::unique_ptr<Element> make_title_info_box();
    std::unique_ptr<Element> make_panel_buttons();
    std::unique_ptr<Element> make_death_main_screen();
    std::unique_ptr<Element> make_stat_screen();
    std::unique_ptr<Element> make_settings_panel();
    std::unique_ptr<Element> make_petal_gallery();
    std::unique_ptr<Element> make_mob_gallery();
    std::unique_ptr<Element> make_changelog();
    std::unique_ptr<Element> make_debug_stats();
    std::unique_ptr<Element> make_github_link_button();
}