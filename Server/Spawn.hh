#pragma once

#include <Shared/Entity.hh>

#include <Shared/Simulation.hh>

Entity &alloc_player(Entity &);
Entity &alloc_petal(uint8_t, Entity &);

void player_spawn(Simulation *, Entity &, Entity &);