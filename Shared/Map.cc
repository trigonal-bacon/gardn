#include <Shared/Map.hh>

#ifdef SERVERSIDE
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#endif

#include <cmath>
#include <iostream>

using namespace Map;

uint32_t Map::difficulty_at_level(uint32_t level) {
    if (level / LEVELS_PER_EXTRA_SLOT > MAX_DIFFICULTY) return MAX_DIFFICULTY;
    return level / LEVELS_PER_EXTRA_SLOT;
}

uint32_t Map::get_zone_from_pos(float x, float y) {
    uint32_t ret = 0;
    for (uint32_t i = 1; i < MAP_DATA.size(); ++i) {
        struct ZoneDefinition const &zone = MAP_DATA[i];
        if (fclamp(x, zone.left, zone.right) == x && fclamp(y, zone.top, zone.bottom) == y)
            ret = i;
    }
    return ret;
}

uint32_t Map::get_suitable_difficulty_zone(uint32_t power) {
    std::vector<uint32_t> possible_zones;
    for (uint32_t i = 0; i < MAP_DATA.size(); ++i)
        if (MAP_DATA[i].difficulty == power) possible_zones.push_back(i);
    if (possible_zones.size() == 0) return 0;
    return possible_zones[frand() * possible_zones.size()];
}

#ifdef SERVERSIDE
#include <Shared/Simulation.hh>
void Map::remove_mob(Simulation *sim, uint32_t zone) {
    DEBUG_ONLY(assert(zone < MAP_DATA.size());)
    --sim->zone_mob_counts[zone];
}

void Map::spawn_random_mob(Simulation *sim) {
    float x = frand() * ARENA_WIDTH;
    float y = frand() * ARENA_HEIGHT;
    uint32_t zone_id = Map::get_zone_from_pos(x, y);
    struct ZoneDefinition const &zone = MAP_DATA[zone_id];
    if (zone.density * (zone.right - zone.left) * (zone.bottom - zone.top) / (500 * 500) < sim->zone_mob_counts[zone_id]) return;
    float sum = 0;
    for (SpawnChance const &s : zone.spawns)
        sum += s.chance;
    sum *= frand();
    for (SpawnChance const &s : zone.spawns) {
        sum -= s.chance;
        if (sum <= 0) {
            Entity &ent = alloc_mob(sim, s.id, x, y, NULL_ENTITY);
            ent.zone = zone_id;
            ent.immunity_ticks = TPS;
            BIT_SET(ent.flags, EntityFlags::kSpawnedFromZone);
            sim->zone_mob_counts[zone_id]++;
            return;
        }
    }
}
#endif