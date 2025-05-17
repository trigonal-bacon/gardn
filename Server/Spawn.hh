#pragma once

#include <Shared/Entity.hh>

#include <Shared/Simulation.hh>

Entity &alloc_drop(PetalID::T);
Entity &alloc_mob(MobID::T, float, float, EntityID);
Entity &alloc_player(Entity &);
Entity &alloc_petal(PetalID::T, Entity &);
Entity &alloc_web(float, Entity &);

void player_spawn(Simulation *, Entity &, Entity &);