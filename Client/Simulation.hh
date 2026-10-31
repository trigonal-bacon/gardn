#pragma once

#include <Shared/Simulation.hh>

class Simulation : public SimulationBase<Simulation> {
public:
    void on_tick();
    void post_tick();
    Entity& force_alloc_ent(const EntityID);
};