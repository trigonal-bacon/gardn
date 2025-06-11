#include <Shared/Map.hh>

#ifdef SERVERSIDE
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#endif

#include <cmath>

using namespace Map;

std::vector<struct ZoneDefinition> const Map::MAP = {
    {
        .x = 5000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 2.5,
        .spawns = {
            { MobID::kBabyAnt, 2 },
            { MobID::kWorkerAnt, 2 },
            { MobID::kRock, 4 },
            { MobID::kLadybug, 1.5 },
            { MobID::kBee, 1.5 },
            { MobID::kCentipede, 0.1 },
            { MobID::kBoulder, 0.04 },
            { MobID::kMassiveLadybug, 0.001 }
        },
        .difficulty = 0,
        .color = 0xff1ea761,
        .name = "Easy"
    },
    {
        .x = 15000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 3.1,
        .spawns = {
            { MobID::kCactus, 5 },
            { MobID::kBee, 1 },
            { MobID::kLadybug, 1},
            { MobID::kSandstorm, 0.2},
            { MobID::kBeetle, 0.2},
            { MobID::kScorpion, 0.1},
            { MobID::kDesertCentipede, 0.02 },
            { MobID::kAntHole, 0.01 },
            { MobID::kShinyLadybug, 0.001 }
        },
        .difficulty = 1,
        .color = 0xffdecf7c,
        .name = "Medium"
    },
    {
        .x = 25000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 3.9,
        .spawns = {
            { MobID::kRock, 4 },
            { MobID::kBoulder, 5 },
            { MobID::kHornet, 2 },
            { MobID::kBee, 2 },
            { MobID::kBeetle, 3 },
            { MobID::kSpider, 2},
            { MobID::kCentipede, 0.1},
            { MobID::kEvilCentipede, 0.1},
            { MobID::kMassiveBeetle, 0.03 },
            { MobID::kAntHole, 0.01 }
        },
        .difficulty = 2,
        .color = 0xffb06655,
        .name = "Hard"
    },
    {
        .x = 35000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 5.0,
        .spawns = {
            { MobID::kBoulder, 3 },
            { MobID::kDarkLadybug, 5 },
            { MobID::kBeetle, 3 },
            { MobID::kHornet, 3 },
            { MobID::kSpider, 2 },
            { MobID::kEvilCentipede, 0.01 },
            { MobID::kMassiveBeetle, 0.01 },
            { MobID::kAntHole, 0.01 },
            { MobID::kRock, 0.1}
        },
        .difficulty = 3,
        .color = 0xff777777,
        .name = "???"
    }
};

SERVER_ONLY(uint32_t ZONE_MOB_COUNTS[256] = {0};)

uint32_t Map::difficulty_at_level(uint32_t level) {
    if (level / 15 > MAX_DIFFICULTY) return MAX_DIFFICULTY;
    return level / 15;
}

uint32_t Map::get_zone_from_pos(float x, float y) {
    uint32_t ret = 0;
    for (uint32_t i = 1; i < Map::MAP.size(); ++i) {
        struct ZoneDefinition const &zone = Map::MAP[i];
        if (fabs(x - zone.x) < zone.w / 2 && fabs(y - zone.y) < zone.h / 2)
            ret = i;
    }
    return ret;
}

#ifdef SERVERSIDE
void Map::remove_mob(uint32_t zone) {
    DEBUG_ONLY(assert(zone < MAP.size());)
    --ZONE_MOB_COUNTS[zone];
}

void Map::spawn_random_mob() {
    float x = frand() * ARENA_WIDTH;
    float y = frand() * ARENA_HEIGHT;
    uint32_t zone_id = Map::get_zone_from_pos(x, y);
    struct ZoneDefinition const &try_zone = Map::MAP[zone_id];
    if (try_zone.density * try_zone.w * try_zone.h / (750 * 750) < ZONE_MOB_COUNTS[zone_id]) return;
    float sum = 0;
    for (SpawnChance const &s : try_zone.spawns)
        sum += s.chance;
    sum *= frand();
    for (SpawnChance const &s : try_zone.spawns) {
        sum -= s.chance;
        if (sum <= 0) {
            MobID::T spawn_id = frand() < 0.00001 ? MobID::kSquare : s.id;
            Entity &ent = alloc_mob(spawn_id, x, y, NULL_ENTITY);
            ent.zone = zone_id;
            ent.flags |= EntityFlags::SpawnedFromZone;
            ZONE_MOB_COUNTS[zone_id]++;
            return;
        }
    }
}
#endif