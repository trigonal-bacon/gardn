#pragma once

#include <Shared/EntityDef.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <functional>
#include <stdint.h>

class Simulation;
class Entity;

static const uint32_t GRID_SIZE = 128 * 2;
static const uint32_t MAX_GRID_X = div_round_up(ARENA_WIDTH, GRID_SIZE);
static const uint32_t MAX_GRID_Y = div_round_up(ARENA_HEIGHT, GRID_SIZE);

static const EntityID::id_type NULL_ID = NULL_ENTITY.id;

class SpatialHash {
public:
    struct EntityCell {
        EntityID::id_type prev_cell = NULL_ID;
        EntityID ent_id = NULL_ENTITY;
    };
    Simulation *simulation;
    EntityID::id_type grid[MAX_GRID_X][MAX_GRID_Y] = {NULL_ID};
    struct EntityCell cells[ENTITY_CAP];
    SpatialHash(Simulation *);
    void clear();
    void insert(EntityID);
    void remove(EntityID);
    void collide(std::function<void(Simulation *, Entity &, Entity &)>);
    void query(float, float, float, float, std::function<void(Simulation *, Entity &)>);
};