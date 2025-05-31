#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

using namespace Ui;

Element *Ui::UiLoadout::petal_tooltips[PetalID::kNumPetals] = {nullptr};

static void make_petal_tooltip(PetalID::T id) {
    Element *tooltip = new Ui::VContainer({
        new Ui::StaticText(20, PETAL_DATA[id].name, { .fill = 0xffffffff, .h_justify = Style::Left }),
        new Ui::StaticText(12, RARITY_NAMES[PETAL_DATA[id].rarity], { .fill = RARITY_COLORS[PETAL_DATA[id].rarity], .h_justify = Style::Left }),
        new Ui::Element(0,10),
        new Ui::StaticText(12, PETAL_DATA[id].description, { .fill = 0xffffffff, .h_justify = Style::Left })
    }, 5, 2);
    tooltip->style.fill = 0x80000000;
    tooltip->style.round_radius = 6;
    tooltip->refactor();
    Ui::UiLoadout::petal_tooltips[id] = tooltip;
}

void Ui::make_petal_tooltips() {
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i)
        make_petal_tooltip(i);
}