#pragma once

#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <cstdint>
#include <vector>

namespace Map {
    const uint32_t ARENA_WIDTH = 40000;
    const uint32_t ARENA_HEIGHT = 4000;
    uint32_t const MAX_DIFFICULTY = 3;
    struct SpawnChance {
        MobID::T const id;
        float const chance;
    };

    struct ZoneDefinition {
        float const x;
        float const y;
        float const w;
        float const h;
        float const density;
        std::vector<struct SpawnChance> const spawns;
        uint32_t const difficulty;
        uint32_t const color;
        char const *name;
    };

    extern std::vector<struct ZoneDefinition> const MAP;

    extern uint32_t difficulty_at_level(uint32_t);
    extern uint32_t get_zone_from_pos(float, float);
    #ifdef SERVERSIDE
    extern void remove_mob(uint32_t);
    extern void spawn_random_mob();
    #endif
}