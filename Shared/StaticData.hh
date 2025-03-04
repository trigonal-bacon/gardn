#pragma once

#include <cstdint>

//MSPT * TPS = 1000
#define MSPT (40)
#define TPS (25)

#define ARENA_WIDTH (1000)
#define ARENA_HEIGHT (1000)

#define PLAYER_ACCELERATION (2)
#define DEFAULT_FRICTION (0.2)

#define MAX_SLOT_COUNT (8)

class PetalID {
public:
    enum {
        kNone,
        kBasic,
        kLight,
        kNumPetals
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
        kMaxRarities
    };
};

struct PetalData {
    char const *name;
    float health;
    float damage;
    float radius;
    float reload;
    float secondary_reload;
    uint8_t count;
    uint8_t rarity;
};

extern struct PetalData PETAL_DATA[PetalID::kNumPetals];