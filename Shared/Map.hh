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
} // namespace Map