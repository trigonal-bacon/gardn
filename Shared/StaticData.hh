#pragma once

#include <Shared/StaticDefinitions.hh>

#include <array>
#include <cstdint>
#include <vector>

extern const uint32_t MAX_LEVEL;
extern const uint32_t TPS;
extern const uint32_t DELETION_ANIMATION_TICKS;

extern const float PETAL_DISABLE_DELAY;
extern const float PLAYER_ACCELERATION;
extern const float DEFAULT_FRICTION;

extern const float SUMMON_RETREAT_RADIUS;

extern const float BASE_FOV;
extern const float BASE_HEALTH;
extern const float BASE_BODY_DAMAGE;

extern struct PetalData const PETAL_DATA[PetalID::kNumPetals];
extern struct MobData const MOB_DATA[MobID::kNumMobs];

extern uint32_t const RARITY_COLORS[RarityID::kNumRarities];
extern char const *RARITY_NAMES[RarityID::kNumRarities];

std::array const MAP = std::to_array<struct ZoneDefinition>({
    {
        .x = 5000,
        .y = 2000,
        .w = 10000,
        .h = 4000,
        .density = 2.5,
        .drop_multiplier = 0.3,
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
        .density = 2.5,
        .drop_multiplier = 0.15,
        .spawns = {
            { MobID::kCactus, 400000 },
            { MobID::kBeetle, 100000 },
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
        .density = 2.5,
        .drop_multiplier = 0.1,
        .spawns = {
            { MobID::kSpider, 100000 },
            { MobID::kBoulder, 100000 },
            { MobID::kBee, 100000 },
            { MobID::kHornet, 100000 },
            { MobID::kBeetle, 50000 },
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
        .density = 2.5,
        .drop_multiplier = 0.025,
        .spawns = {
            { MobID::kDarkLadybug, 150000 },
            { MobID::kBeetle, 150000 },
            { MobID::kHornet, 150000 },
            { MobID::kSpider, 150000 },
            { MobID::kBoulder, 100000 },
            { MobID::kEvilCentipede, 10000 },
            { MobID::kMassiveBeetle, 2500 },
            { MobID::kAntHole, 2500 },
            { MobID::kSquare, 1 }
        },
        .difficulty = 3,
        .color = 0xff777777,
        .name = "???"
    }
});

extern StaticArray<float, MAX_DROPS_PER_MOB> const &get_mob_drop_chances(MobID::T);

extern uint32_t score_to_pass_level(uint32_t);
extern uint32_t score_to_level(uint32_t);
extern uint32_t level_to_score(uint32_t);
extern uint32_t loadout_slots_at_level(uint32_t);

extern float hp_at_level(uint32_t);