#include <Client/Ui/InGame/Loadout.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/StaticData.hh>

#include <Client/Game.hh>

#include <format>
#include <array>

using namespace Ui;

std::array<std::unique_ptr<Element>, PetalID::kNumPetals> Ui::UiLoadout::petal_tooltips;

static float get_reload_factor() {
    if (!Game::alive()) return 1;
    float factor = 1;
    Entity &player = Game::simulation.get_ent(Game::player_id);
    for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
        factor *= PETAL_DATA[player.get_loadout_ids(i)].attributes.extra_reload_factor;
    }
    return factor;
}

static float get_damage_factor() {
    if (!Game::alive()) return 1;
    float factor = 1;
    Entity &player = Game::simulation.get_ent(Game::player_id);
    for (uint32_t i = 0; i < player.get_loadout_count(); ++i) {
        factor *= PETAL_DATA[player.get_loadout_ids(i)].attributes.extra_damage_factor;
    }
    return factor;
}

static std::unique_ptr<Element> make_petal_stat_container(PetalID::T id) {
    std::vector<std::unique_ptr<Ui::Element>> stats = make_children(el<Element>(0,10));
    struct PetalData const &petal_data = PETAL_DATA[id];
    struct PetalAttributes const &attrs = petal_data.attributes;
    if (petal_data.health > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Health:", Style{ .fill = 0xff77ff77 }),
            el<StaticText>(12, format_number(petal_data.health))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (petal_data.damage > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Damage:", Style{ .fill = 0xffff7777 }),
            el<DynamicText>(12, [&](){
                return format_number(petal_data.damage * get_damage_factor());
            })
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.armor > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Armor:", Style{ .fill = 0xff777777 }),
            el<StaticText>(12, format_number(attrs.armor))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.damage_reflection > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Damage Reflection:", Style{ .fill = 0xff777777 }),
            el<StaticText>(12, format_pct(attrs.damage_reflection * 100))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.constant_heal > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Heal:", Style{ .fill = 0xffff96cb }),
            el<StaticText>(12, format_number(attrs.constant_heal) + "/s")
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.burst_heal > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Heal:", Style{ .fill = 0xffff96cb }),
            el<StaticText>(12, format_number(attrs.burst_heal))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.poison_damage.damage > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Poison:", Style{ .fill = 0xffce76db }),
            el<StaticText>(12, format_number(attrs.poison_damage.time * attrs.poison_damage.damage) + " (" + format_number(attrs.poison_damage.damage) + "/s)")
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_health > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Flower Health:", Style{ .fill = 0xff77ff77 }),
            el<StaticText>(12, format_number(attrs.extra_health))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_body_damage > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Body Damage:", Style{ .fill = 0xffff7777 }),
            el<StaticText>(12, format_number(attrs.extra_body_damage))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.poison_armor > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Poison Armor:", Style{ .fill = 0xffce76db }),
            el<StaticText>(12, format_number(attrs.poison_armor) + "/s")
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_rotation_speed > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Rotation Speed:", Style{ .fill = 0xffcde23b }),
            el<StaticText>(12, "+" + format_number(attrs.extra_rotation_speed) + " rad/s")
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.spawns != MobID::kNumMobs && attrs.spawn_count == 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Contents:", Style{ .fill = 0xffd2eb34 }),
            el<StaticText>(12, (petal_data.count > 1 ? format_number(petal_data.count) + "x " : "") + (MOB_DATA[attrs.spawns].name))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.spawns != MobID::kNumMobs && attrs.spawn_count > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Spawns:", Style{ .fill = 0xffd2eb34 }),
            el<StaticText>(12, format_number(attrs.spawn_count) + "x " + (MOB_DATA[attrs.spawns].name))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.vision_factor < 1) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Extra Vision:", Style{ .fill = 0xffcde23b }),
            el<StaticText>(12, format_pct(100 / attrs.vision_factor))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_range > 0) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Attack Range:", Style{ .fill = 0xffcde23b }),
            el<StaticText>(12, "+" + format_number(attrs.extra_range))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_damage_factor > 1) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Extra Damage:", Style{ .fill = 0xffff7777 }),
            el<StaticText>(12, "+"+format_pct(100 * (attrs.extra_damage_factor - 1)))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    if (attrs.extra_reload_factor > 1) {
        stats.push_back(el<HContainer>(make_children(
            el<StaticText>(12, "Extra Reload:", Style{ .fill = 0xff7777ff }),
            el<StaticText>(12, "+"+format_pct(100 * (attrs.extra_reload_factor - 1)))
        ), 0, 5, Style{ .h_justify = Style::Left }));
    }
    return el<VContainer>(std::move(stats), 0, 2, Style{ .h_justify = Style::Left });
}

static void make_petal_tooltip(PetalID::T id) {
    auto tooltip = static_cast<std::unique_ptr<Element>>(el<VContainer>(make_children(
        #ifdef DEBUG
        el<HFlexContainer>(
            el<StaticText>(20, PETAL_DATA[id].name, Style{ .fill = 0xffffffff, .h_justify = Style::Left }),
            el<DynamicText>(16, [=](){
                float reload = PETAL_DATA[id].reload * get_reload_factor();
                std::string rld_str = reload == 0 ? "" :
                    PETAL_DATA[id].attributes.secondary_reload == 0 ? std::format("{:.1f}s ⟳", reload) : 
                    std::format("{:.1f} + {:.1f}s ⟳", reload, PETAL_DATA[id].attributes.secondary_reload);
                return rld_str;
            }, Style{ .fill = 0xffffffff, .v_justify = Style::Top }),
            5, 10, Style{}
        ),
        #else
        el<StaticText>(20, PETAL_DATA[id].name, Style{ .fill = 0xffffffff, .h_justify = Style::Left }),
        #endif
        el<StaticText>(14, RARITY_NAMES[PETAL_DATA[id].rarity], Style{ .fill = RARITY_COLORS[PETAL_DATA[id].rarity], .h_justify = Style::Left }),
        el<Element>(0,8),
        el<StaticText>(12, PETAL_DATA[id].description, Style{ .fill = 0xffffffff, .h_justify = Style::Left })
        DEBUG_ONLY(,make_petal_stat_container(id))
    ), 5, 2));
    tooltip->style.fill = 0x80000000;
    tooltip->style.round_radius = 6;
    tooltip->refactor();
    Ui::UiLoadout::petal_tooltips[id] = std::move(tooltip);
}

void Ui::make_petal_tooltips() {
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i)
        make_petal_tooltip(i);
}