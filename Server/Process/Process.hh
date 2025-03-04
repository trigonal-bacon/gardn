#pragma once

class Simulation;
class Entity;

void tick_entity_motion(Simulation *, Entity &);
void tick_player_entities(Simulation *);
