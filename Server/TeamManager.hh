#pragma once

#include <Helpers/Array.hh>

#include <Shared/Entity.hh>
#include <Shared/StaticDefinitions.hh>

class Simulation;

class TeamManager {
    StaticArray<EntityID, 4> teams;
    Simulation *simulation;
public:
    TeamManager(Simulation *);
    void add_team(uint8_t);
    EntityID const get_random_team() const;
};