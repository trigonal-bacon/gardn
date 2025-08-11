#pragma once

#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <array>
#include <cstdint>
#include <vector>

SERVER_ONLY(class Simulation;)

namespace Map {
    extern uint32_t difficulty_at_level(uint32_t);
    extern uint32_t get_zone_from_pos(float, float);
    #ifdef SERVERSIDE
    extern void remove_mob(Simulation *, uint32_t);
    extern void spawn_random_mob(Simulation *);
    #endif
}