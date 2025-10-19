#include <Shared/EntityDef.hh>

#include <Shared/Simulation.hh>
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

PetalID::T LoadoutSlot::get_petal_id() const {
    return id;
}

void LoadoutSlot::update_id(Simulation *sim, PetalID::T petal_id) {
    struct PetalData const &petal_data = PETAL_DATA[id];
    for (uint32_t j = 0; j < size(); ++j) {
        LoadoutPetal &petal_slot = petals[j];
        if (sim->ent_alive(petal_slot.ent_id))
            sim->request_delete(petal_slot.ent_id);
    }
    reset();
    id = petal_id;
}

void LoadoutSlot::force_reload() {
    already_spawned = 1;
    for (uint32_t j = 0; j < size(); ++j)
        petals[j].reload = PETAL_DATA[id].reload * TPS;
}

uint32_t LoadoutSlot::size() const {
    if (PETAL_DATA[id].attributes.split_projectile)
        return 1;
    return std::min(static_cast<uint32_t>(PETAL_DATA[id].count), MAX_PETALS_IN_CLUMP);
}