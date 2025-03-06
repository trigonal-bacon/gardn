#pragma once

#include <cstdint>
#include <vector>

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
        kHeavy,
        kStinger,
        kLeaf,
        kTwin,
        kRose,
        kMissile,
        kTriplet,
        kAntEgg,
        kBeetleEgg,
        kTringer,
        kNumPetals
    };
};

class MobID {
public:
    enum : uint8_t {
        kBabyAnt,
        kWorkerAnt,
        kSoldierAnt,
        kBee,
        kLadybug,
        kBeetle,
        kMassiveLadybug,
        kMassiveBeetle,
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
    enum {
        kPassiveRot,
        kNoRot,
        kFollowRot
    };
    float clump_radius;
    float secondary_reload;
    float constant_heal;
    float burst_heal;
    uint8_t defend_only;
    float icon_angle;
    uint8_t rotation_style;
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
    std::vector<MobDrop> drops;
};

extern struct PetalData PETAL_DATA[PetalID::kNumPetals];
extern struct MobData MOB_DATA[MobID::kNumMobs];

extern uint32_t RARITY_COLORS[RarityID::kNumRarities];