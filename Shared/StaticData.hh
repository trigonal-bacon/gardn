#pragma once

#include <Shared/Helpers.hh>

#include <cstdint>
#include <vector>

//MSPT * TPS = 1000
#define TPS (25)

#define ARENA_WIDTH (5000)
#define ARENA_HEIGHT (500)

#define PLAYER_ACCELERATION (2.5)
#define DEFAULT_FRICTION (0.25)

#define SUMMON_RETREAT_RADIUS (600)

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
        kIris,
        kMissile,
        kDandelion,
        kBubble,
        kFaster,
        kWeb,
        kWing,
        kTriplet,
        kAntEgg,
        kBeetleEgg,
        kTringer,
        kShield,
        kThirdEye,
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
        kHornet,
        kCactus,
        kRock,
        kCentipede,
        kSpider,
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
        kReturning,
        kBasicAggro
    };
};

struct Poison {
    float damage;
    float time;
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
    struct Poison poison_damage;
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

struct MobAttributes {
    float aggro_radius = 600;
    uint8_t segments = 0;
};

struct MobDrop {
    uint8_t id;
    float chance;
};

struct MobData {
    char const *name;
    uint8_t rarity;
    RangeValue health;
    float damage;
    RangeValue radius;
    float xp;
    std::vector<MobDrop> drops;
    struct MobAttributes attributes;
};

extern struct PetalData PETAL_DATA[PetalID::kNumPetals];
extern struct MobData MOB_DATA[MobID::kNumMobs];

extern uint32_t RARITY_COLORS[RarityID::kNumRarities];