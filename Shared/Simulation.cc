#include <Shared/Simulation.hh>
#include <Shared/Helpers.hh>

#include <iostream>

Simulation::Simulation() SERVER_ONLY(: spatial_hash(this)) {}

void Simulation::reset() {
    active_entities.clear();
    pending_delete.clear();
    for (uint32_t i = 0; i < ENTITY_CAP; ++i) { 
        hash_tracker[i] = entity_tracker[i] = 0;
        entities[i].init();
    }
}

Entity &Simulation::alloc_ent() {
    for (uint32_t i = 1; i < ENTITY_CAP; ++i) {
        if (entity_tracker[i]) continue;
        entity_tracker[i] = 1;
        DEBUG_ONLY(std::cout << "ent_create <" << hash_tracker[i] << ',' << i << ">\n";)
        entities[i].init();
        entities[i].id = EntityId(i, hash_tracker[i]);
        return entities[i];
    }
    assert(!"Entity cap reached");
}

Entity &Simulation::get_ent(EntityId &id) {
    assert(ent_exists(id));
    return entities[id.id];
}

void Simulation::force_alloc_ent(EntityId &id) {
    assert(id.id < ENTITY_CAP);
    DEBUG_ONLY(std::cout << "ent_create <" << id.hash << ',' << id.id << ">\n";)
    assert(!entity_tracker[id.id]);
    entities[id.id].init();
    entity_tracker[id.id] = 1;
    hash_tracker[id.id] = id.hash;
    entities[id.id].id = id;
}

uint8_t Simulation::ent_exists(EntityId &id) {
    assert(id.id < ENTITY_CAP);
    return entity_tracker[id.id] && hash_tracker[id.id] == id.hash;
}

uint8_t Simulation::ent_alive(EntityId &id) {
    return ent_exists(id) && entities[id.id].pending_delete == 0;
}

void Simulation::request_delete(EntityId &id) {
    DEBUG_ONLY(std::cout << "ent_request_delete <" << id.hash << ',' << id.id << ">\n";)
    assert(ent_exists(id));
    if (entities[id.id].pending_delete) return;
    entities[id.id].pending_delete = 1;
    pending_delete.push(id);
}

void Simulation::delete_ent(EntityId &id) {
    DEBUG_ONLY(std::cout << "ent_delete <" << id.hash << ',' << id.id << ">\n";)
    assert(ent_exists(id));
    entity_tracker[id.id] = 0;
    ++hash_tracker[id.id];
}

void Simulation::pre_tick() {
    active_entities.clear();
    for (uint32_t i = 0; i < ENTITY_CAP; ++i) {
        if (!entity_tracker[i]) continue;
        active_entities.push(entities[i].id);
    }
}