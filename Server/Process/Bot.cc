#include <Server/Bot.hh>
#include <Server/Process.hh>

#include <Server/Spawn.hh>
#include <Server/EntityFunctions.hh>
#include <Server/PetalTracker.hh>
#include <Shared/Entity.hh>
#include <Shared/Simulation.hh>
#include <Shared/Map.hh>
#include <Shared/StaticData.hh>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>

// ---------------- Bot types ----------------
// 0 = balanced (default, 90%), 
// 1 = right-biased wanderer (5%), 
// 2 = monotype (5%)
static inline uint8_t bot_kind(Entity const &player) {
    EntityID cam = player.get_parent();
    // Use camera id to keep kind stable per session
    uint32_t r = (uint32_t)(cam.id % 100); // range 0–99
    if (r < 90) return 1;   // 90% chance
    if (r < 95) return 2;   // next 5%
    return 2;               // last 5%
}


// Minimum zone difficulty based on level
static inline uint32_t min_difficulty_for_level(uint32_t lvl) {
    if (lvl >= 20) return 3;   // no Easy/Med/Hard
    if (lvl >= 12) return 2;   // no Easy/Med
    if (lvl >= 5) return 1;   // no Easy
    return 0;
}

// Mirror client XP gains for deleting petals (see Server/Client.cc)
constexpr uint32_t RARITY_TO_XP[RarityID::kNumRarities] = { 2, 10, 50, 200, 1000, 5000, 0 };

// Detect if the player is on a zone border line; if so, we will bias movement to the right to avoid back-and-forth jitter.
static bool is_on_zone_border(float x, float y) {
    const float eps = 3.0f;
    for (auto const &zone : MAP_DATA) {
        // Vertical borders
        if (y >= zone.top - eps && y <= zone.bottom + eps) {
            if (fabsf(x - zone.left) <= eps) return true;
            if (fabsf(x - zone.right) <= eps) return true;
        }
        // Horizontal borders
        if (x >= zone.left - eps && x <= zone.right + eps) {
            if (fabsf(y - zone.top) <= eps) return true;
            if (fabsf(y - zone.bottom) <= eps) return true;
        }
    }
    return false;
}

// Rarity base weights; tune as needed
static float rarity_weight(uint8_t rarity) {
    switch (rarity) {
        case RarityID::kCommon:     return 1.0f;
        case RarityID::kUnusual:    return 2.0f;
        case RarityID::kRare:       return 3.5f;
        case RarityID::kEpic:       return 0.0f;
        case RarityID::kLegendary:  return 0.0f;
        case RarityID::kMythic:     return 0.0f;
        case RarityID::kUnique:     return 0.0f;
        default:                    return 0.0f;
    }
}

// Simple petal score (kept intentionally simple)
static float petal_score(PetalID::T id) {
    if (id == PetalID::kNone) return 0.0f;
    if (id >= PetalID::kNumPetals) return 0.0f;
    auto const &pd = PETAL_DATA[id];
    float score = rarity_weight(pd.rarity) * 10.0f;
    score += pd.damage * 0.4f;
    score += pd.attributes.constant_heal * 6.0f;
    if (pd.count > 1) score += (pd.count - 1) * 1.0f;
    return score;
}

static uint8_t has_any_petals(Entity &player) {
    uint32_t active = player.get_loadout_count();
    uint32_t total = active + MAX_SLOT_COUNT;
    for (uint32_t i = 0; i < total; ++i)
        if (player.get_loadout_ids(i) != PetalID::kNone) return 1;
    return 0;
}

static void ensure_basics_if_empty(Entity &player) {
    if (has_any_petals(player)) return;
    uint32_t active = player.get_loadout_count();
    for (uint32_t i = 0; i < active; ++i)
        player.set_loadout_ids(i, PetalID::kBasic);
}

// Rebalance active slots with best available petals
static void rebalance_loadout(Entity &player) {
    uint32_t active = player.get_loadout_count();
    uint32_t total = active + MAX_SLOT_COUNT;
    if (active == 0 || total == 0) return;

    struct SlotScore { uint32_t idx; float score; PetalID::T id; };
    SlotScore scores[2 * MAX_SLOT_COUNT];
    uint32_t n = 0;

    for (uint32_t i = 0; i < total; ++i) {
        PetalID::T id = player.get_loadout_ids(i);
        float s = petal_score(id);
        scores[n++] = { i, s, id };
    }
    std::sort(scores, scores + n, [](auto const &a, auto const &b){ return a.score > b.score; });

    PetalID::T desired[2 * MAX_SLOT_COUNT] = { PetalID::kNone };
    for (uint32_t pos = 0; pos < active && pos < n; ++pos)
        desired[pos] = scores[pos].id;

    uint32_t write = active;
    for (uint32_t k = active; k < n && write < (active + MAX_SLOT_COUNT); ++k)
        desired[write++] = scores[k].id;

    for (uint32_t i = 0; i < total; ++i)
        player.set_loadout_ids(i, desired[i]);
}

// Monotype: prefer filling active slots with the highest scoring petal the bot owns
static void monoize_loadout(Entity &player) {
    uint32_t active = player.get_loadout_count();
    uint32_t total  = active + MAX_SLOT_COUNT;
    if (total == 0) return;

    PetalID::T best_id = PetalID::kNone;
    float best_s = -1.0f;
    for (uint32_t i = 0; i < total; ++i) {
        PetalID::T id = player.get_loadout_ids(i);
        float s = petal_score(id);
        if (s > best_s) { best_s = s; best_id = id; }
    }
    if (best_id == PetalID::kNone) return;

    for (uint32_t i = 0; i < active; ++i)
        player.set_loadout_ids(i, best_id);
}

static void trash_petal(Simulation *sim, Entity &player, uint32_t pos) {
    if (pos >= player.get_loadout_count() + MAX_SLOT_COUNT) return;
    PetalID::T old_id = player.get_loadout_ids(pos);
    if (old_id == PetalID::kNone) return;

    // Award score for non-basic petals and track deletion, mirroring client behavior
    if (old_id != PetalID::kBasic) {
        uint8_t rarity = PETAL_DATA[old_id].rarity;
        player.set_score(player.get_score() + RARITY_TO_XP[rarity]);
        if (player.deleted_petals.size() == player.deleted_petals.capacity()) {
            PetalTracker::remove_petal(sim, player.deleted_petals[0]);
        }
        player.deleted_petals.push_back(old_id);
    }

    player.set_loadout_ids(pos, PetalID::kNone);
}

static void maybe_trash_unwanted(Simulation *sim, Entity &player) {
    uint32_t active = player.get_loadout_count();
    uint32_t total = active + MAX_SLOT_COUNT;
    if (active == 0) return;

    float worst_active = 1e9f;
    for (uint32_t i = 0; i < active; ++i) {
        PetalID::T id = player.get_loadout_ids(i);
        worst_active = std::min(worst_active, petal_score(id));
    }
    if (worst_active == 1e9f) worst_active = 0.0f;

    float trash_threshold = worst_active * 0.5f;
    for (uint32_t i = active; i < total; ++i) {
        PetalID::T id = player.get_loadout_ids(i);
        if (id == PetalID::kNone) continue;
        float s = petal_score(id);
        if (s < trash_threshold) {
            trash_petal(sim, player, i);
            break;
        }
    }
}

static bool has_empty_slot(Entity &player) {
    uint32_t active = player.get_loadout_count();
    uint32_t total = active + MAX_SLOT_COUNT;
    for (uint32_t i = 0; i < total; ++i)
        if (player.get_loadout_ids(i) == PetalID::kNone) return true;
    return false;
}

static void worst_slots(Entity &player, float &worst_active_score, int &worst_active_idx,
                        float &worst_reserve_score, int &worst_reserve_idx) {
    uint32_t active = player.get_loadout_count();
    uint32_t total = active + MAX_SLOT_COUNT;
    worst_active_score = 1.0e9f; worst_active_idx = -1;
    worst_reserve_score = 1.0e9f; worst_reserve_idx = -1;

    for (uint32_t i = 0; i < active; ++i) {
        float s = petal_score(player.get_loadout_ids(i));
        if (s < worst_active_score) { worst_active_score = s; worst_active_idx = (int)i; }
    }
    for (uint32_t i = active; i < total; ++i) {
        PetalID::T id = player.get_loadout_ids(i);
        if (id == PetalID::kNone) continue;
        float s = petal_score(id);
        if (s < worst_reserve_score) { worst_reserve_score = s; worst_reserve_idx = (int)i; }
    }
}

static EntityID find_nearest_mob(Simulation *sim, Entity const &player, float radius) {
    EntityID best = NULL_ENTITY;
    float min_dist = radius;
    sim->spatial_hash.query(player.get_x(), player.get_y(), radius, radius, [&](Simulation *sim2, Entity &ent){
        if (!sim2->ent_alive(ent.id)) return;
        if (!ent.has_component(kMob)) return;
        if (ent.get_team() == player.get_team()) return;
        if (ent.immunity_ticks > 0) return;
        float d = Vector(ent.get_x() - player.get_x(), ent.get_y() - player.get_y()).magnitude();
        if (d < min_dist) { min_dist = d; best = ent.id; }
    });
    return best;
}

static EntityID find_best_drop(Simulation *sim, Entity const &player, float radius, float &best_drop_score) {
    EntityID best = NULL_ENTITY;
    best_drop_score = 0.0f;
    sim->spatial_hash.query(player.get_x(), player.get_y(), radius, radius, [&](Simulation *sim2, Entity &ent){
        if (!sim2->ent_alive(ent.id)) return;
        if (!ent.has_component(kDrop)) return;
        if (ent.immunity_ticks > 0) return;
        PetalID::T drop_id = ent.get_drop_id();
        if (drop_id >= PetalID::kNumPetals || drop_id == PetalID::kNone) return;
        float s = petal_score(drop_id);
        if (s > best_drop_score) {
            best_drop_score = s;
            best = ent.id;
        }
    });
    return best;
}

// Load bot names from Server/Process/ndat.txt (and fallbacks) once
static std::vector<std::string> &bot_names() {
    static std::vector<std::string> names;
    static bool loaded = false;
    if (!loaded) {
        const char *candidates[] = {
            "Server/Process/ndat.txt",
            "../Server/Process/ndat.txt",
            "../../Server/Process/ndat.txt",
            "Server/ndat.txt",
            "../Server/ndat.txt",
            "../../Server/ndat.txt",
            "ndat.txt"
        };
        for (auto p : candidates) {
            std::ifstream f(p);
            if (!f.good()) continue;
            std::string line;
            while (std::getline(f, line)) {
                if (!line.empty() && line.size() < 64) names.push_back(line);
            }
            if (!names.empty()) break;
        }
        if (names.empty()) names.push_back("Unnamed Flower");
        loaded = true;
    }
    return names;
}

// Track session end tick per-bot (by camera id) so names persist ~1 hour
static std::unordered_map<EntityID::id_type, game_tick_t> &bot_session_expiry() {
    static std::unordered_map<EntityID::id_type, game_tick_t> m;
    return m;
}

#ifndef WASM_SERVER
void ensure_bot_name(Simulation *sim, Entity &player) {
    if (!sim->ent_alive(player.get_parent())) return;
    Entity &camera = sim->get_ent(player.get_parent());
    auto &expiry = bot_session_expiry()[camera.id.id];
    game_tick_t now = player.lifetime;
    bool need_new = player.get_name().empty() || (expiry != 0 && now >= expiry);
    if (need_new) {
        auto &names = bot_names();
        size_t idx = (size_t)(frand() * names.size()) % names.size();
        player.set_name(names[idx]);
        game_tick_t base = (game_tick_t)(TPS * 3600);
        game_tick_t jitter = (game_tick_t)(TPS * (600.0f * (frand() - 0.5f)));
        expiry = now + base + jitter;
    }
}
#endif

static void _ensure_spawn(Simulation *sim, Entity &player) {
    if (!sim->ent_alive(player.get_parent())) return;
    ensure_bot_name(sim, player);
    player.set_loadout_count(loadout_slots_at_level(score_to_level(player.get_score())));
    ensure_basics_if_empty(player);
}

static void engage_with_petals(Simulation *sim, Entity &player, EntityID target_id) {
    BitMath::set(player.input, InputFlags::kAttacking);
    if (!sim->ent_alive(target_id)) return;
    Entity &t = sim->get_ent(target_id);
    Vector delta(t.get_x() - player.get_x(), t.get_y() - player.get_y());
    float dist = delta.magnitude();
    if (dist == 0) {
        player.acceleration = Vector::rand(PLAYER_ACCELERATION);
        return;
    }
    const float desired = player.get_radius() + 100.0f;
    const float too_close = desired - 25.0f;
    const float too_far  = desired + 25.0f;
    Vector dir = delta; dir.normalize();
    Vector tangent(-dir.y, dir.x);
    float orbit_sign = (player.id.id % 2 == 0) ? 1.0f : -1.0f;
    if (dist > too_far) {
        player.acceleration = dir * (PLAYER_ACCELERATION * 1.0f);
    } else if (dist < too_close) {
        player.acceleration = (dir * -1.0f) * (PLAYER_ACCELERATION * 1.0f);
    } else {
        player.acceleration = tangent * (PLAYER_ACCELERATION * 0.8f * orbit_sign);
    }
    player.set_angle(player.acceleration.angle());
}

static uint8_t maybe_relocate_to_zone(Simulation *sim, Entity &player) {
    uint32_t level = score_to_level(player.get_score());
    uint32_t desired_difficulty = Map::difficulty_at_level(level);
    // Enforce minimum difficulty thresholds by level
    uint32_t min_diff = min_difficulty_for_level(level);
    if (desired_difficulty < min_diff) desired_difficulty = min_diff;
    uint32_t current_zone_idx = Map::get_zone_from_pos(player.get_x(), player.get_y());
    ZoneDefinition const &curr_zone = MAP_DATA[current_zone_idx];
    if (curr_zone.difficulty >= desired_difficulty) return 0;
    float eager = player.get_overlevel_timer() > 0.5f * TPS ? 1.0f : 0.2f;
    if (frand() > eager) return 0;
    uint32_t target_zone_idx = Map::get_suitable_difficulty_zone(desired_difficulty);
    ZoneDefinition const &target_zone = MAP_DATA[target_zone_idx];
    float tx = 0.5f * (target_zone.left + target_zone.right);
    float ty = 0.5f * (target_zone.top + target_zone.bottom);
    player.input = 0;
    Vector v(tx - player.get_x(), ty - player.get_y());
    if (v.magnitude() > 0) v.set_magnitude(PLAYER_ACCELERATION);
    player.acceleration = v;
    player.set_angle(v.angle());
    return 1;
}

void tick_bot_player_behavior(Simulation *sim, Entity &player) {
    if (player.pending_delete) return;
    if (!player.has_component(kFlower)) return;
    if (!sim->ent_alive(player.get_parent())) return;

    _ensure_spawn(sim, player);

    uint8_t kind = bot_kind(player);

    // If on a zone border line, move right to avoid jitter on boundary edges
    if (is_on_zone_border(player.get_x(), player.get_y())) {
        player.input = 0;
        Vector right(1.0f, 0.0f);
        right.set_magnitude(PLAYER_ACCELERATION * (kind == 1 ? 1.2f : 1.0f));
        player.acceleration = right;
        player.set_angle(0.0f);
        ++player.ai_tick;
        return;
    }

    if (player.ai_tick % TPS == 0) {
        if (kind == 2) monoize_loadout(player);
        else           rebalance_loadout(player);
        maybe_trash_unwanted(sim, player);
    }

    ensure_basics_if_empty(player);

    float best_drop_score = 0.0f;
    EntityID best_drop = find_best_drop(sim, player, 900.0f, best_drop_score);

    float worst_active = 1e9f; int worst_active_idx = -1;
    float worst_reserve = 1e9f; int worst_reserve_idx = -1;
    worst_slots(player, worst_active, worst_active_idx, worst_reserve, worst_reserve_idx);
    if (worst_active == 1e9f) worst_active = 0.0f;
    if (worst_reserve == 1e9f) worst_reserve = 0.0f;

    float base_margin = (worst_active < 25.0f) ? 0.5f : 2.5f;
    uint8_t prioritize_drop = 0;
    uint8_t room_available = has_empty_slot(player);

    if (!best_drop.null() && best_drop_score > worst_active + base_margin) {
        if (!room_available) {
            if (worst_reserve_idx >= 0 && best_drop_score > worst_reserve + 0.5f) {
                trash_petal(sim, player, (uint32_t)worst_reserve_idx);
                room_available = 1;
            } else if (worst_active_idx >= 0 && best_drop_score > worst_active + 4.0f) {
                trash_petal(sim, player, (uint32_t)worst_active_idx);
                room_available = 1;
            }
        }
        prioritize_drop = room_available ? 1 : 0;
    }

    if (prioritize_drop) {
        player.input = 0;
        Entity &d = sim->get_ent(best_drop);
        Vector v(d.get_x() - player.get_x(), d.get_y() - player.get_y());
        if (v.magnitude() > 0) v.set_magnitude(PLAYER_ACCELERATION);
        player.acceleration = v;
        player.set_angle(v.angle());
        ++player.ai_tick;
        return;
    }

    EntityID target = NULL_ENTITY;
    float best_damage = 0.0f;
    const game_tick_t now = player.lifetime;
    const game_tick_t window = (game_tick_t)(2 * TPS);

    for (uint32_t i = 0; i < 8; ++i) {
        EntityID target = sim->ent_alive(player.last_damaged_by) ? player.last_damaged_by : NULL_ENTITY;
        if (target.null()) target = find_nearest_mob(sim, player, 900.0f);
    }

    if (target.null()) {
        target = find_nearest_mob(sim, player, 900.0f);
    }

    if (target.null() && !prioritize_drop) {
        player.input = 0;
        Vector right(1.0f, 0.0f);
        float speed = (bot_kind(player) == 1 ? 1.0f : 0.6f);
        right.set_magnitude(PLAYER_ACCELERATION * speed);
        player.acceleration = right;
        player.set_angle(0.0f);
    }

    if (maybe_relocate_to_zone(sim, player)) {
        ++player.ai_tick;
        return;
    }

    if (sim->ent_alive(target)) {
        player.input = 0;
        engage_with_petals(sim, player, target);
        if (bot_kind(player) == 1) {
            player.acceleration.x += PLAYER_ACCELERATION * 0.2f;
        }
    }

    ++player.ai_tick;
}