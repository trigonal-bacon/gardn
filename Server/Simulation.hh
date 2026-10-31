#pragma once

#include <Shared/Simulation.hh>

#include <Server/SpatialHash.hh>

class Simulation : public SimulationBase<Simulation> {
    friend class PetalTracker;
    std::array<uint32_t, PetalID::kNumPetals> petal_count_tracker;
    std::array<uint32_t, MAP_DATA.size()> zone_mob_counts;
public:
    SpatialHash spatial_hash;
    Simulation() : SimulationBase<Simulation>(), spatial_hash(*this) {}
    void on_tick();
    void post_tick();
    void remove_mob(uint32_t);
    void spawn_random_mob(float, float);
    /* finds a spawn location at least <d> units from a player,
    and places it in the Vector &. returns whether or not a
    suitable spawn location was found */
    bool find_spawn_location(float, Vector&);
};