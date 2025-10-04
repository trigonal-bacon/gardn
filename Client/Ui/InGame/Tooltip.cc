#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/StaticData.hh>

#include <format>
#include <cmath>
#include <vector>
#include <string>

using namespace Ui;

Element *Ui::UiLoadout::petal_tooltips[PetalID::kNumPetals] = {nullptr};

static void make_petal_tooltip(PetalID::T id) {
    PetalData const &d = PETAL_DATA[id];
    PetalAttributes const &a = d.attributes;

    auto format_number = [](float v) {
        return (std::floor(v) == v)
            ? std::format("{:.0f}", v)
            : std::format("{:.1f}", v);
    };

    auto positive = [](float v) { return v > 0.0001f; };

    std::string rld_str = d.reload == 0 ? "" :
        a.secondary_reload == 0 ? std::format("{}s ⟳", format_number(d.reload)) :
        std::format("{}s + {}s ⟳", format_number(d.reload), format_number(a.secondary_reload));

    // Derived effects inferred from gameplay logic (server-side buffs)
    float derived_extra_health       = 0.0f;
    float derived_extra_range        = 0.0f;
    float derived_vision_factor      = 0.0f; // e.g. 1.67 means 167%
    float derived_armor              = 0.0f;
    float derived_reflect_pct        = 0.0f;
    float derived_poison_armor_per_s = 0.0f;
    float derived_size_pct           = 0.0f; // e.g. +20% size
    uint8_t derived_is_poisonous     = 0;
    bool derived_basil_heal_note     = false;

    switch (id) {
        case PetalID::kAntennae:
            derived_vision_factor = 1.0f / (1.0f - 0.4f); // ~1.67x
            break;
        case PetalID::kObserver:
            derived_vision_factor = 1.0f / (1.0f - 0.75f); // 4.0x
            break;
        case PetalID::kThirdEye:
            derived_extra_range = 65.0f;
            break;
        case PetalID::kCactus:
            derived_extra_health = 20.0f;
            break;
        case PetalID::kTricac:
            derived_extra_health = 60.0f;
            break;
        case PetalID::kSoil:
            derived_extra_health = 35.0f;
            derived_size_pct = 20.0f; // size multiplier +20%
            break;
        case PetalID::kPoisonCactus:
            derived_extra_health = 20.0f;
            derived_is_poisonous = 1;
            break;
        case PetalID::kBone:
            derived_armor = 8.0f;
            break;
        case PetalID::kSalt:
            derived_reflect_pct = 25.0f; 
            break;
        case PetalID::kLotus:
            derived_poison_armor_per_s = 3.5f; // per second
            break;
        case PetalID::kBasil:
            // Tooltip note only; actual multiplier applied server-side harmonically
            derived_basil_heal_note = true;
            break;
        default:
            break;
    }

    // Build rows dynamically (skip zero/empty stats)
    std::vector<Element *> rows;

    // Header (name + reload)
    rows.push_back(
        new Ui::HFlexContainer(
            new Ui::StaticText(20, d.name, { .fill = 0xffffffff, .h_justify = Style::Left }),
            new Ui::StaticText(16, rld_str, { .fill = 0xffffffff, .v_justify = Style::Top }),
            5, 10, {}
        )
    );
    rows.push_back(new Ui::StaticText(14, RARITY_NAMES[d.rarity], { .fill = RARITY_COLORS[d.rarity], .h_justify = Style::Left }));
    rows.push_back(new Ui::Element(0,10));
    rows.push_back(new Ui::StaticText(12, d.description, { .fill = 0xffffffff, .h_justify = Style::Left }));

    bool has_base_stats = positive(d.health) || positive(d.damage) || positive(a.constant_heal) || positive(a.burst_heal) || positive(a.poison_damage.damage);
    if (has_base_stats) rows.push_back(new Ui::Element(0,10));

    if (positive(d.health)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Health: ", { .fill = 0xff66ff66, .h_justify = Style::Left }),
                new Ui::StaticText(12, format_number(d.health), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(d.damage)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Damage: ", { .fill = 0xffff6666, .h_justify = Style::Left }),
                new Ui::StaticText(12, format_number(d.damage), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(a.constant_heal)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Heal: ", { .fill = 0xffff94c9, .h_justify = Style::Left }),
                new Ui::StaticText(12, format_number(a.constant_heal) + "/s", { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(a.burst_heal)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Heal: ", { .fill = 0xffff94c9, .h_justify = Style::Left }),
                new Ui::StaticText(12, format_number(a.burst_heal), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(a.poison_damage.damage)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Poison: ", { .fill = 0xffce76db, .h_justify = Style::Left }),
                new Ui::StaticText(12,
                    format_number(a.poison_damage.damage * a.poison_damage.time) +
                    (a.poison_damage.time ? " (" + format_number(a.poison_damage.damage) + "/s)" : ""),
                    { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (a.spawns != MobID::kNumMobs) {
        uint32_t total = (a.spawn_count ? a.spawn_count : 1) * (d.count ? d.count : 1);
        std::string qty = total > 1 ? std::to_string(total) + "x " : "";
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Summons: ", { .fill = 0xffd2eb34, .h_justify = Style::Left }),
                new Ui::StaticText(12, qty + MOB_DATA[a.spawns].name, { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    // Derived effects section (only show non-zero)
    bool has_derived = positive(derived_extra_health) || positive(derived_extra_range) ||
                       positive(derived_vision_factor - 1.0f) || positive(derived_armor) ||
                       positive(derived_reflect_pct) || positive(derived_poison_armor_per_s) ||
                       positive(derived_size_pct) || derived_is_poisonous || derived_basil_heal_note;

    if (has_derived) rows.push_back(new Ui::Element(0,10));

    if (positive(derived_extra_health)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Flower Health: ", { .fill = 0xff66ff66, .h_justify = Style::Left }),
                new Ui::StaticText(12, "+" + format_number(derived_extra_health), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_extra_range)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Attack Range: ", { .fill = 0xffcde23b, .h_justify = Style::Left }),
                new Ui::StaticText(12, "+" + format_number(derived_extra_range), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_vision_factor - 1.0f)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Extra Vision: ", { .fill = 0xffcde23b, .h_justify = Style::Left }),
                new Ui::StaticText(12, std::format("{:.0f}%", derived_vision_factor * 100.0f), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_armor)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Armor: ", { .fill = 0xffcde23b, .h_justify = Style::Left }),
                new Ui::StaticText(12, format_number(derived_armor), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_reflect_pct)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Damage Reflection: ", { .fill = 0xffcde23b, .h_justify = Style::Left }),
                new Ui::StaticText(12, std::format("{:.0f}%", derived_reflect_pct), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_poison_armor_per_s)) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Poison Resistance: ", { .fill = 0xffcde23b, .h_justify = Style::Left }),
                new Ui::StaticText(12, std::format("{:.1f}/s", derived_poison_armor_per_s), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (positive(derived_size_pct)) {
        // Brown fill of Soil petal (from draw code): 0xff695118
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Size: ", { .fill = 0xff695118, .h_justify = Style::Left }),
                new Ui::StaticText(12, std::format("+{:.0f}%", derived_size_pct), { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (derived_is_poisonous) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Poisonous Body", { .fill = 0xffcde23b, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    if (derived_basil_heal_note) {
        rows.push_back(
            new Ui::HContainer({
                new Ui::StaticText(12, "Healing Buff: ", { .fill = 0xffff94c9, .h_justify = Style::Left }),
                new Ui::StaticText(12, "+35%", { .fill = 0xffffffff, .h_justify = Style::Left })
            }, 0, 0, { .h_justify = Style::Left })
        );
    }

    // Build final container
    Element *tooltip = new Ui::VContainer(rows, 5, 2);
    tooltip->style.fill = 0x80000000;
    tooltip->style.round_radius = 6;
    tooltip->refactor();
    Ui::UiLoadout::petal_tooltips[id] = tooltip;
}

void Ui::make_petal_tooltips() {
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i)
        make_petal_tooltip(i);
}