#pragma once

#include <Helpers/Array.hh>
#include <Helpers/Math.hh>

#include <cstdint>
#include <string_view>

inline const uint32_t ARENA_WIDTH = 40000;
inline const uint32_t ARENA_HEIGHT = 4000;

inline const uint32_t MAX_SLOT_COUNT = 8;
inline const uint32_t LEVELS_PER_EXTRA_SLOT = 15;
inline const uint32_t LEADERBOARD_SIZE = 10;
inline const uint32_t MAX_PETALS_IN_CLUMP = 4;
inline const uint32_t MAX_DIFFICULTY = 3;
inline const uint32_t MAX_DROPS_PER_MOB = 6;

namespace DamageType {
enum : uint8_t { kContact, kPoison, kReflect };
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
  kYggdrasil,
  kRice,
  kBone,
  kYucca,
  kCorn,
  kGoldenLeaf,
  kNumPetals
};
}; // namespace PetalID

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
}; // namespace MobID

namespace RarityID {
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

namespace ColorID {
enum { kYellow, kGray, kBlue, kRed, kNumColors };
};

namespace AIState {
enum { kIdle, kIdleMoving, kReturning, kBasicAggro };
};

namespace EntityFlags {
enum {
  kIsDespawning,
  kNoFriendlyCollision,
  kTakesNoKnockback,
  kDieOnParentDeath,
  kSpawnedFromZone,
  kNoDrops,
  kHasCulling,
  kIsCulled,
  kCPUControlled,
  kIsDetached
};
};

namespace FaceFlags {
enum {
  kAttacking,
  kDefending,
  kPoisoned,
  kDandelioned,
  kDeadEyes,
  kSquareEyes
};
};

namespace EquipmentFlags {
enum { kThirdEye, kAntennae, kObserver, kCutter, kNone };
};

namespace InputFlags {
enum { kAttacking, kDefending };
}

struct PoisonDamage {
  float damage;
  float time;
};

struct PetalAttributes {
  enum { kPassiveRot, kNoRot, kFollowRot };
  float attack_range = 105;
  float neutral_range = 40;
  float defend_range = 15;
  float clump_radius = 0;
  float secondary_reload = 0;
  float constant_heal = 0;
  float burst_heal = 0;
  float density = 1.0;
  float armor = 0;
  float poison_armor = 0;
  float dandelion_inflict_seconds = 0;
  float slow_inflict_seconds = 0;
  float vision_factor = 1;
  float extra_body_damage = 0;
  float extra_rotation_speed = 0;
  float extra_range = 0;
  float extra_health = 0;
  float damage_reflection = 0;
  float extra_damage_factor = 1;
  float extra_reload_factor = 1;
  struct PoisonDamage poison_damage;
  uint8_t defend_only = 0;
  float icon_angle = 0;
  uint8_t split_projectile = 0;
  uint8_t rotation_style = kPassiveRot;
  uint8_t spawns = MobID::kNumMobs;
  uint8_t spawn_count = 0;
  uint8_t equipment = EquipmentFlags::kNone;
};

struct PetalData {
  std::string_view name;
  std::string_view description;
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
  float density = 1;
  // relative to player
  float passive_speed_mult = 0.4;
  float aggro_speed_mult = 0.96;
  uint8_t unmovable = 0;
  uint8_t segments = 1;
  struct PoisonDamage poison_damage;
};

struct MobData {
  std::string_view name;
  std::string_view description;
  uint8_t rarity;
  RangeValue health;
  float damage;
  RangeValue radius;
  uint32_t xp;
  StaticArray<PetalID::T, MAX_DROPS_PER_MOB> drops;
  struct MobAttributes attributes;
};

struct SpawnChance {
  MobID::T id;
  float chance;
};

struct ZoneDefinition {
  float left;
  float top;
  float right;
  float bottom;
  float density;
  float drop_multiplier;
  StaticArray<struct SpawnChance, MobID::kNumMobs> spawns;
  uint32_t difficulty;
  uint32_t color;
  std::string_view name;
};
