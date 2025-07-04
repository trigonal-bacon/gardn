#include <Shared/Simulation.hh>
#include <Shared/Helpers.hh>

#ifdef DEBUG
#include <iostream>
#endif

Simulation::Simulation() SERVER_ONLY(: spatial_hash(this)) {}

void Simulation::reset() {
    active_entities.clear();
    for (EntityID::id_type i = 0; i < ENTITY_CAP; ++i) { 
        hash_tracker[i] = entity_tracker[i] = 0;
        entities[i].init();
    }
    arena_info.init();
}

Entity &Simulation::alloc_ent() {
    for (EntityID::id_type i = 1; i < ENTITY_CAP; ++i) {
        if (entity_tracker[i]) continue;
        entity_tracker[i] = 1;
        entities[i].init();
        DEBUG_ONLY(std::cout << "ent_create <" << hash_tracker[i] << ',' << i << ">\n";)
        entities[i].id = EntityID(i, hash_tracker[i]);
        return entities[i];
    }
    assert(!"Entity cap reached");
}

Entity &Simulation::get_ent(EntityID const &id) {
    DEBUG_ONLY(assert(ent_exists(id));)
    return entities[id.id];
}

void Simulation::force_alloc_ent(EntityID const &id) {
    assert(id.id < ENTITY_CAP);
    DEBUG_ONLY(std::cout << "ent_create <" << id.hash << ',' << id.id << ">\n";)
    assert(!entity_tracker[id.id]);
    entities[id.id].init();
    entity_tracker[id.id] = 1;
    hash_tracker[id.id] = id.hash;
    entities[id.id].id = id;
}

uint8_t Simulation::ent_exists(EntityID const &id) const {
    DEBUG_ONLY(assert(id.id < ENTITY_CAP);)
    return entity_tracker[id.id] && hash_tracker[id.id] == id.hash;
}

uint8_t Simulation::ent_alive(EntityID const &id) const {
    return ent_exists(id) && entities[id.id].pending_delete == 0
    SERVER_ONLY(&& entities[id.id].deletion_tick == 0);
}

void Simulation::request_delete(EntityID const &id) {
    //DEBUG_ONLY(std::cout << "ent_request_delete <" << id.hash << ',' << id.id << ">\n";)
    DEBUG_ONLY(assert(ent_exists(id)));
    entities[id.id].pending_delete = 1;
}

void Simulation::_delete_ent(EntityID const &id) {
    DEBUG_ONLY(std::cout << "ent_delete <" << id.hash << ',' << id.id << ">\n";)
    DEBUG_ONLY(assert(ent_exists(id)));
    entity_tracker[id.id] = 0;
    hash_tracker[id.id]++;
}

void Simulation::pre_tick() {
    active_entities.clear();
    for (EntityID::id_type i = 0; i < ENTITY_CAP; ++i) {
        if (!entity_tracker[i]) continue;
        active_entities.push(entities[i].id.id);
    }
}

void Simulation::for_each_entity(std::function<void(Simulation *, Entity &)> cb) { \
    for (EntityID::id_type i = 0; i < active_entities.size(); ++i) { \
        Entity &ent = entities[active_entities[i]]; \
        cb(this, ent); \
    } \
}

#define COMPONENT(name) \
template<> \
void Simulation::for_each<k##name>(std::function<void(Simulation *, Entity &)> cb) { \
    for (EntityID::id_type i = 0; i < active_entities.size(); ++i) { \
        Entity &ent = entities[active_entities[i]]; \
        SERVER_ONLY(if (ent.pending_delete) continue;) \
        if (ent.has_component(k##name)) cb(this, ent); \
    } \
}
PERCOMPONENT
#undef COMPONENT