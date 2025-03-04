#pragma once

#include <cstdint>

//MSPT * TPS = 1000
#define MSPT (40)
#define TPS (25)

#define ARENA_WIDTH (1000)
#define ARENA_HEIGHT (1000)

#define PLAYER_ACCELERATION (2)
#define DEFAULT_FRICTION (0.2)

class PetalID {
public:
    enum {
        kNone,
        kBasic,
        kLight,
        kNumPetals
    };
};

struct PetalData {
    char const *name;
    float health;
    float damage;
    float reload;
    float secondary_reload;
    uint32_t count;
};

extern struct PetalData PETAL_DATA[PetalId::kNumPetals];

