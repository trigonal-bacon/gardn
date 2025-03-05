#pragma once

#include <cstdint>

//MSPT * TPS = 1000
#define MSPT (40)
#define TPS (25)

#define ARENA_WIDTH (1000)
#define ARENA_HEIGHT (1000)

#define PLAYER_ACCELERATION (2.5)
#define DEFAULT_FRICTION (0.25)

#define MAX_SLOT_COUNT (8)

class PetalID {
public:
    enum {
        kNone,
        kBasic,
        kLight,
        kTwin,
        kTriplet,
        kNumPetals
    };
};

class MobID {
public:
    enum : uint8_t {
        kBabyAnt,
        kWorkerAnt,
        kNumMobs
    };
};

class RarityID {
public:
    enum {
        kCommon,
        kUnusual,
        kRare,
        kEpic,
        kLegendary,
        kMythic,
        kNumRarities
    };
};

class AIState {
public:
    enum {
        kIdle,
        kIdleMoving,
        kBasicAggro
    };
};

struct PetalAttributes {
    float clump_radius;
    float secondary_reload;
};

struct PetalData {
    char const *name;
    float health;
    float damage;
    float radius;
    float reload;
    uint8_t count;
    uint8_t rarity;
    struct PetalAttributes attributes;
};

struct MobDrop {
    uint8_t id;
    float chance;
};

struct MobData {
    char const *name;
    uint8_t rarity;
    float health;
    float damage;
    float radius;
    float xp;
    MobDrop drops[6];
};

extern struct PetalData PETAL_DATA[PetalID::kNumPetals];
extern struct MobData MOB_DATA[MobID::kNumMobs];

extern uint32_t RARITY_COLORS[RarityID::kNumRarities];