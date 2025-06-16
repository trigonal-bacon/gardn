#include <Shared/Map.hh>

#ifdef SERVERSIDE
#include <Server/Spawn.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>
#endif

#include <cmath>
#include <iostream>

using namespace Map;

std::vector<struct ZoneDefinition> const Map::MAP = {
    {
        .x = 5000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 2.5,
        .drop_multiplier = 0.2,
        .spawns = {
            { MobID::kRock, 500000 },
            { MobID::kLadybug, 100000 },
            { MobID::kBee, 100000 },
            { MobID::kBabyAnt, 25000 },
            { MobID::kCentipede, 10000 },
            { MobID::kBoulder, 10000 },
            { MobID::kMassiveLadybug, 200 },
            { MobID::kSquare, 1 }
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
        .density = 3,
        .drop_multiplier = 0.1,
        .spawns = {
            { MobID::kCactus, 500000 },
            { MobID::kBeetle, 150000 },
            { MobID::kSandstorm, 50000 },
            { MobID::kBee, 50000 },
            { MobID::kScorpion, 50000 },
            { MobID::kLadybug, 50000 },
            { MobID::kDesertCentipede, 10000 },
            { MobID::kAntHole, 2000 },
            { MobID::kShinyLadybug, 1000 },
            { MobID::kSquare, 1 }
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
        .density = 3.5,
        .drop_multiplier = 0.05,
        .spawns = {
            { MobID::kRock, 150000 },
            { MobID::kBeetle, 150000 },
            { MobID::kSpider, 100000 },
            { MobID::kBoulder, 100000 },
            { MobID::kBee, 100000 },
            { MobID::kHornet, 50000 },
            { MobID::kLadybug, 50000 },
            { MobID::kCentipede, 10000 },
            { MobID::kEvilCentipede, 10000 },
            { MobID::kMassiveBeetle, 2000 },
            { MobID::kAntHole, 2000 },
            { MobID::kSquare, 1 }
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
        .density = 3.5,
        .drop_multiplier = 0.025,
        .spawns = {
            { MobID::kBoulder, 250000 },
            { MobID::kDarkLadybug, 150000 },
            { MobID::kBeetle, 150000 },
            { MobID::kHornet, 100000 },
            { MobID::kSpider, 100000 },
            { MobID::kEvilCentipede, 25000 },
            { MobID::kMassiveBeetle, 2000 },
            { MobID::kAntHole, 2000 },
            { MobID::kSquare, 1 }
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
            MobID::T spawn_id = s.id;
            Entity &ent = alloc_mob(spawn_id, x, y, NULL_ENTITY);
            ent.zone = zone_id;
            BIT_SET(ent.flags, EntityFlags::kSpawnedFromZone);
            ZONE_MOB_COUNTS[zone_id]++;
            return;
        }
    }
}
#endif

std::array<std::vector<float>, MobID::kNumMobs> const Map::get_auto_petal_drops() {
    std::array<std::vector<float>, MobID::kNumMobs> ret = {};
    double const RARITY_MULT[RarityID::kNumRarities] = {60000,20000,2500,100,5,2.5,1};
    double MOB_SPAWN_RATES[MobID::kNumMobs] = {0};
    double PETAL_AGGREGATE_DROPS[PetalID::kNumPetals] = {0};
    for (struct ZoneDefinition const &zone : MAP) {
        double total = 0;
        for (SpawnChance const &s : zone.spawns) total += s.chance;
        for (SpawnChance const &s : zone.spawns) {
            double xx = (s.chance * zone.drop_multiplier / total);
            MOB_SPAWN_RATES[s.id] += xx;
            if (s.id == MobID::kAntHole) {
                MOB_SPAWN_RATES[MobID::kQueenAnt] += xx;
                MOB_SPAWN_RATES[MobID::kDigger] += 0.1 * xx;
                MOB_SPAWN_RATES[MobID::kSoldierAnt] += 15 * xx;
                MOB_SPAWN_RATES[MobID::kWorkerAnt] += 10 * xx;
                MOB_SPAWN_RATES[MobID::kBabyAnt] += 5 * xx;
            }
        }
    }

    for (MobID::T id = 0; id < MobID::kNumMobs; ++id)
        for (PetalID::T drop_id : MOB_DATA[id].drops) PETAL_AGGREGATE_DROPS[drop_id]++;

    double BASE_NUM = MOB_SPAWN_RATES[MobID::kSquare];

    for (MobID::T id = 0; id < MobID::kNumMobs; ++id) {
        for (PetalID::T drop_id : MOB_DATA[id].drops) {
            float chance = fclamp((BASE_NUM * RARITY_MULT[PETAL_DATA[drop_id].rarity]) / (PETAL_AGGREGATE_DROPS[drop_id] * MOB_SPAWN_RATES[id] * MOB_DATA[id].attributes.segments), 0, 1);
            ret[id].push_back(chance);
        }
    }
    return ret;
}