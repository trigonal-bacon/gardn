#include <Shared/EntityDef.hh>

#include <Shared/StaticData.hh>

EntityID::EntityID() {
    id = hash = 0;
}

EntityID::EntityID(uint16_t id, uint16_t hash) : id(id), hash(hash) {
}

bool EntityID::null() const {
    return id == 0;
}

void EntityID::operator=(const EntityID o) {
    id = o.id;
    hash = o.hash;
}

bool operator<(const EntityID a, const EntityID b) {
    return (a.id * 65536 + a.hash) < (b.id * 65536 + b.hash);
}

bool operator==(const EntityID a, const EntityID b) {
    return a.id == b.id && a.hash == b.hash;
}

LoadoutSlot::LoadoutSlot() {
    reset();
}

void LoadoutSlot::reset() {
    id = PetalID::kNone;
    already_spawned = 0;
    for (uint32_t i = 0; i < MAX_PETALS_IN_CLUMP; ++i) {
        petals[i].reload = 0;
        petals[i].ent_id = NULL_ENTITY;
    }
}