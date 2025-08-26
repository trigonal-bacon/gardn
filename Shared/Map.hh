#pragma once

#include <Shared/StaticData.hh>

#include <array>
#include <cstdint>
#include <vector>

#ifdef SERVERSIDE
#include <Helpers/Vector.hh>
#endif

SERVER_ONLY(class Simulation;)

namespace Map {
    extern uint32_t difficulty_at_level(uint32_t);
    extern uint32_t get_zone_from_pos(float, float);
    extern uint32_t get_suitable_difficulty_zone(uint32_t);
    #ifdef SERVERSIDE
    extern void remove_mob(Simulation *, uint32_t);
    extern void spawn_random_mob(Simulation *, float, float);
    /* finds a spawn location at least <d> units from a player,
    and places it in the Vector &. returns whether or not a
    suitable spawn location was found */ 
    extern bool find_spawn_location(Simulation *, float, Vector &);
    #endif
}