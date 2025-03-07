#pragma once

#include <Shared/EntityDef.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <functional>
#include <vector>
#include <stdint.h>

class Simulation;
class Entity;

static const uint32_t GRID_SIZE = 128 * 2;
static const uint32_t MAX_GRID_X = div_round_up(ARENA_WIDTH, GRID_SIZE);
static const uint32_t MAX_GRID_Y = div_round_up(ARENA_HEIGHT, GRID_SIZE);
class SpatialHash {
public:
    Simulation *simulation;
    std::vector<EntityID> cells[MAX_GRID_X][MAX_GRID_Y];
    SpatialHash(Simulation *);
    void clear();
    void insert(Entity &);
    void collide(std::function<void(Simulation *, Entity &, Entity &)>);
    void query(float, float, float, float, std::function<void(Simulation *, Entity &)>);
};