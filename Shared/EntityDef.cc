#include <Shared/EntityDef.hh>

#include <Shared/StaticDefinitions.hh>

EntityID::EntityID() : id(0), hash(0) {}

EntityID::EntityID(id_type id, hash_type hash) : id(id), hash(hash) {}

bool EntityID::null() const {
    return id == 0;
}

uint32_t EntityID::make_hash(EntityID const o) {
    return o.id * 65536 + o.hash;
}

bool EntityID::equal_to(EntityID const a, EntityID const b) {
    return a.id == b.id && a.hash == b.hash;
}

bool operator<(EntityID const a, EntityID const b) {
    return EntityID::make_hash(a) < EntityID::make_hash(b);
}

bool operator==(EntityID const a, EntityID const b) {
    return EntityID::equal_to(a, b);
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