#pragma once

#include <Shared/Helpers.hh>

#include <cstdint>
#include <vector>

//MSPT * TPS = 1000
#define TPS (25)

#define ARENA_WIDTH (4000)
#define ARENA_HEIGHT (2000)

#define PLAYER_ACCELERATION (3.0)
#define DEFAULT_FRICTION (0.25)

#define SUMMON_RETREAT_RADIUS (600)

#define BASE_FOV (1.0f)
#define BASE_HEALTH (100.0f)

#define MAX_SLOT_COUNT (8)

namespace PetalID {
    typedef uint8_t T;
    enum : T {
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
        kRock,
        kCactus,
        kWeb,
        kWing,
        kPeas,
        kSand,
        kPincer,
        kDahlia,
        kTriplet,
        kAntEgg,
        kBlueIris,
        kPollen,
        kPoisonPeas,
        kBeetleEgg,
        kAzalea,
        kStick,
        kTringer,
        kTriweb,
        kAntennae,
        kTricac,
        kShield,
        kThirdEye,
        kObserver,
        kNumPetals
    };
};

namespace MobID {
    typedef uint8_t T;
    enum : T {
        kBabyAnt,
        kWorkerAnt,
        kSoldierAnt,
        kBee,
        kLadybug,
        kBeetle,
        kMassiveLadybug,
        kMassiveBeetle,
        kDarkLadybug,
        kHornet,
        kCactus,
        kRock,
        kCentipede,
        kEvilCentipede,
        kDesertCentipede,
        kSandstorm,
        kScorpion,
        kSpider,
        kAntHole,
        kQueenAnt,
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
    uint8_t spawns = MobID::kNumMobs;
    uint8_t spawn_count;
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
    float aggro_radius = 500;
    uint8_t segments;
    uint8_t stationary;
    struct Poison poison_damage;
};

struct MobDrop {
    PetalID::T id;
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

extern float scoreToPassLevel(uint32_t);
extern uint32_t scoreToLevel(float);
extern float levelToScore(uint32_t);
extern uint32_t loadOutSlotsAtLevel(uint32_t);

extern float hpAtLevel(uint32_t);