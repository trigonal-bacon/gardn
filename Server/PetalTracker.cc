#include <Server/PetalTracker.hh>

#include <Shared/Simulation.hh>

using namespace PetalTracker;

void PetalTracker::add_petal(Simulation *sim, PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return;
    ++sim->petal_count_tracker[id];
}

void PetalTracker::remove_petal(Simulation *sim, PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return;
    --sim->petal_count_tracker[id];
}

uint32_t PetalTracker::get_count(Simulation *sim, PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return 0;
    return sim->petal_count_tracker[id];
}
