#pragma once

#include <Shared/Helpers.hh>

#include <cstdint>
#include <vector>

extern const uint32_t SERVER_PORT;
extern const uint32_t MAX_LEVEL;
const uint32_t ENTITY_CAP = 32768;
const uint32_t MAX_SLOT_COUNT = 8;
const uint32_t LEADERBOARD_SIZE = 10;

extern const uint32_t TPS;
extern const uint32_t DELETION_ANIMATION_TICKS;

extern const float PETAL_DISABLE_DELAY;
extern const float PLAYER_ACCELERATION;
extern const float DEFAULT_FRICTION;

extern const float SUMMON_RETREAT_RADIUS;

extern const float BASE_FOV;
extern const float BASE_HEALTH;

namespace DamageType {
    enum : uint8_t {
        kContact,
        kPoison,
        kReflect
    };
}

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
        kHeaviest,
        kThirdEye,
        kObserver,
        kPoisonCactus,
        kSalt,
        kUniqueBasic,
        kSquare,
        kMoon,
        kLotus,
        kCutter,
        kYinYang,
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
        kBoulder,
        kCentipede,
        kEvilCentipede,
        kDesertCentipede,
        kSandstorm,
        kScorpion,
        kSpider,
        kAntHole,
        kQueenAnt,
        kShinyLadybug,
        kSquare,
        kDigger,
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
        kUnique,
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

struct PoisonDamage {
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
    struct PoisonDamage poison_damage;
    uint8_t spawns = MobID::kNumMobs;
    uint8_t spawn_count;
};

struct PetalData {
    char const *name;
    char const *description;
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
    uint8_t segments;
    uint8_t stationary;
    struct PoisonDamage poison_damage;
};

struct MobDrop {
    PetalID::T id;
    float chance;
};

struct MobData {
    char const *name;
    char const *description;
    uint8_t rarity;
    RangeValue health;
    float damage;
    RangeValue radius;
    uint32_t xp;
    std::vector<MobDrop> const drops;
    struct MobAttributes attributes;
};

extern struct PetalData const PETAL_DATA[PetalID::kNumPetals];
extern struct MobData const MOB_DATA[MobID::kNumMobs];

extern uint32_t const RARITY_COLORS[RarityID::kNumRarities];
extern char const *RARITY_NAMES[RarityID::kNumRarities];

extern uint32_t score_to_pass_level(uint32_t);
extern uint32_t score_to_level(uint32_t);
extern uint32_t level_to_score(uint32_t);
extern uint32_t loadout_slots_at_level(uint32_t);

extern float hp_at_level(uint32_t);