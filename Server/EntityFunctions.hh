#pragma once

#include <Shared/EntityDef.hh>

class Simulation;
class Entity;
class EntityID;

void inflict_damage(Simulation *, EntityID const, EntityID const, float, uint8_t);
void inflict_heal(Simulation *, Entity &, float);

void entity_on_death(Simulation *, Entity const &);

EntityID find_nearest_enemy(Simulation *, Entity const &, float);

void entity_set_owner(Entity &, EntityID const);
void entity_set_base(Entity &, EntityID const);