#pragma once

#include <Shared/Entity.hh>

#include <functional>

class Simulation;

Entity& alloc_drop(Simulation&, PetalID::T);
Entity& alloc_mob(Simulation&, MobID::T, float, float, const EntityID,
                  std::function<void(Entity&)> = nullptr);
Entity& alloc_player(Simulation&, const EntityID);
Entity& alloc_petal(Simulation&, PetalID::T, const Entity&);
Entity& alloc_web(Simulation&, float, const Entity&);
Entity& alloc_cpu_camera(Simulation&, const EntityID);

void player_spawn(Simulation&, Entity&, Entity&);