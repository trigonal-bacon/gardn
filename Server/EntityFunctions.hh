#pragma once

#include <Shared/EntityDef.hh>

class Simulation;
class Entity;
class EntityID;

void inflict_damage(Simulation *, EntityID const &, Entity &, float);
void inflict_heal(Simulation *, Entity &, float);

void entity_on_death(Simulation *, Entity &);

EntityID find_nearest_enemy(Simulation *, Entity &, float);

void entity_set_owner(Entity &, EntityID const);