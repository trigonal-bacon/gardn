#include <Server/PetalTracker.hh>

#include <Shared/Helpers.hh>

#include <iostream>

using namespace PetalTracker;

static uint32_t COUNT_TRACKER[PetalID::kNumPetals] = {0};

void PetalTracker::add_petal(PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return;
    ++COUNT_TRACKER[id];
}

void PetalTracker::remove_petal(PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return;
    --COUNT_TRACKER[id];
}

uint32_t PetalTracker::get_count(PetalID::T id) {
    DEBUG_ONLY(assert(id < PetalID::kNumPetals);)
    if (id == PetalID::kNone) return 0;
    return COUNT_TRACKER[id];
}
