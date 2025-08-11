#pragma once

#include <Shared/StaticData.hh>

class Simulation;

namespace PetalTracker {
    void add_petal(Simulation *, PetalID::T);
    void remove_petal(Simulation *, PetalID::T);
    uint32_t get_count(Simulation *, PetalID::T);
}