#pragma once

#include <Shared/EntityDef.hh>

class Simulation;
class Entity;

void inflict_damage(Simulation *, Entity &, Entity &, float);
void inflict_heal(Simulation *, Entity &, float);

void entity_on_death(Simulation *, Entity &);

EntityId find_nearest_enemy(Simulation *, Entity &, float);