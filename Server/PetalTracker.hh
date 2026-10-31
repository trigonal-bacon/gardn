#pragma once

#include <Shared/StaticData.hh>

class Simulation;

class PetalTracker {
public:
    static void add_petal(Simulation&, PetalID::T);
    static void remove_petal(Simulation&, PetalID::T);
    static uint32_t get_count(Simulation&, PetalID::T);
};
