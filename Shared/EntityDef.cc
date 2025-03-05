#include <Shared/EntityDef.hh>

#include <Shared/StaticData.hh>

#include <iostream>

EntityId NULL_ENTITY;

EntityId::EntityId() {
    id = hash = 0;
}

EntityId::EntityId(uint16_t id, uint16_t hash) : id(id), hash(hash) {
}

bool EntityId::null() const {
    return id == 0;
}

void EntityId::operator=(const EntityId &o) {
    id = o.id;
    hash = o.hash;
}

bool operator<(const EntityId &a, const EntityId &b) {
    return (a.id * 65536 + a.hash) < (b.id * 65536 + b.hash);
}

bool operator==(const EntityId &a, const EntityId &b) {
    return a.id == b.id && a.hash == b.hash;
}

void EntityId::print() {
    std::cout << '<' << hash << ',' << id << '>';
}

LoadoutSlot::LoadoutSlot() {
    reset();
}

void LoadoutSlot::reset() {
    id = PetalID::kNone;
    already_spawned = 0;
    for (uint32_t i = 0; i < 3; ++i) {
        petals[i].reload = 0;
        petals[i].ent_id = NULL_ENTITY;
    }
}