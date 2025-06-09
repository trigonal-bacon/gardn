#pragma once

#include <Shared/Entity.hh>

#include <Shared/Simulation.hh>

Entity &alloc_drop(PetalID::T);
Entity &alloc_mob(MobID::T, float, float, EntityID const);
Entity &alloc_player(EntityID const);
Entity &alloc_petal(PetalID::T, Entity const &);
Entity &alloc_web(float, Entity const &);

void player_spawn(Simulation *, Entity &, Entity &);