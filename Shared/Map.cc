#include <Shared/Map.hh>

#ifdef SERVERSIDE
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#endif

#include <cmath>
#include <iostream>

using namespace Map;

SERVER_ONLY(uint32_t ZONE_MOB_COUNTS[MAP.size()] = {0};)

uint32_t Map::difficulty_at_level(uint32_t level) {
    if (level / 15 > MAX_DIFFICULTY) return MAX_DIFFICULTY;
    return level / 15;
}

uint32_t Map::get_zone_from_pos(float x, float y) {
    uint32_t ret = 0;
    for (uint32_t i = 1; i < MAP.size(); ++i) {
        struct ZoneDefinition const &zone = MAP[i];
        if (fabs(x - zone.x) < zone.w / 2 && fabs(y - zone.y) < zone.h / 2)
            ret = i;
    }
    return ret;
}

#ifdef SERVERSIDE
#include <Shared/Simulation.hh>
void Map::remove_mob(uint32_t zone) {
    DEBUG_ONLY(assert(zone < MAP.size());)
    --ZONE_MOB_COUNTS[zone];
}

void Map::spawn_random_mob(Simulation *sim) {
    float x = frand() * ARENA_WIDTH;
    float y = frand() * ARENA_HEIGHT;
    uint32_t zone_id = Map::get_zone_from_pos(x, y);
    struct ZoneDefinition const &zone = MAP[zone_id];
    if (zone.density * zone.w * zone.h / (100 * 100) < ZONE_MOB_COUNTS[zone_id]) return;
    float sum = 0;
    for (SpawnChance const &s : zone.spawns)
        sum += s.chance;
    sum *= frand();
    for (SpawnChance const &s : zone.spawns) {
        sum -= s.chance;
        if (sum <= 0) {
            MobID::T spawn_id = s.id;
            Entity &ent = alloc_mob(sim, spawn_id, x, y, NULL_ENTITY);
            ent.zone = zone_id;
            BIT_SET(ent.flags, EntityFlags::kSpawnedFromZone);
            ZONE_MOB_COUNTS[zone_id]++;
            return;
        }
    }
}
#endif