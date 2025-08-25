#include <Shared/Simulation.hh>
#include <Shared/Helpers.hh>

#ifdef DEBUG
#include <iostream>

std::ostream &operator<<(std::ostream &out, EntityID const &id) { 
    return out << '<' << (uint32_t) id.hash << ',' << (uint32_t) id.id << '>';
}
#endif

Simulation::Simulation() SERVER_ONLY(: spatial_hash(this)) {
    reset();
}

void Simulation::reset() {
    active_entities.clear();
    for (EntityID::id_type i = 0; i < ENTITY_CAP; ++i) { 
        hash_tracker[i] = 0;
        BIT_UNSET_ARR(entity_tracker, i);
        entities[i].init();
    }
    arena_info.init();
    #ifdef SERVERSIDE
    spatial_hash.refresh(ARENA_WIDTH, ARENA_HEIGHT);
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i)
        petal_count_tracker[i] = 0;
    for (uint32_t i = 0; i < MAP_DATA.size(); ++i)
        zone_mob_counts[i] = 0;
    #endif
}

Entity &Simulation::alloc_ent() {
    for (EntityID::id_type i = 1; i < ENTITY_CAP; ++i) {
        if (BIT_AT_ARR(entity_tracker, i)) continue;
        BIT_SET_ARR(entity_tracker, i);
        entities[i].init();
        DEBUG_ONLY(std::cout << "ent_create " << EntityID(i, hash_tracker[i]) << "\n";)
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
    DEBUG_ONLY(std::cout << "ent_create " << id << "\n";)
    assert(!BIT_AT_ARR(entity_tracker, id.id));
    entities[id.id].init();
    BIT_SET_ARR(entity_tracker, id.id);
    hash_tracker[id.id] = id.hash;
    entities[id.id].id = id;
}

uint8_t Simulation::ent_exists(EntityID const &id) const {
    DEBUG_ONLY(assert(id.id < ENTITY_CAP);)
    return BIT_AT_ARR(entity_tracker, id.id) && hash_tracker[id.id] == id.hash;
}

uint8_t Simulation::ent_alive(EntityID const &id) const {
    return ent_exists(id) && !entities[id.id].pending_delete
    SERVER_ONLY(&& entities[id.id].deletion_tick == 0);
}

void Simulation::request_delete(EntityID const &id) {
    DEBUG_ONLY(assert(ent_exists(id)));
    entities[id.id].pending_delete = 1;
}

void Simulation::_delete_ent(EntityID const &id) {
    DEBUG_ONLY(std::cout << "ent_delete " << id << "\n";)
    DEBUG_ONLY(assert(ent_exists(id)));
    BIT_UNSET_ARR(entity_tracker, id.id);
    hash_tracker[id.id]++;
}

void Simulation::tick() {
    active_entities.clear();
    for (EntityID::id_type i = 1; i < ENTITY_CAP; ++i) {
        if (!BIT_AT_ARR(entity_tracker, i)) continue;
        active_entities.push(entities[i].id.id);
    }
    on_tick();
}

void Simulation::for_each_entity(std::function<void(Simulation *, Entity &)> cb) { \
    for (EntityID::id_type i = 0; i < active_entities.size(); ++i) { \
        if (!BIT_AT_ARR(entity_tracker, active_entities[i])) continue; \
        Entity &ent = entities[active_entities[i]]; \
        cb(this, ent); \
    } \
}

#define COMPONENT(name) \
template<> \
void Simulation::for_each<k##name>(std::function<void(Simulation *, Entity &)> cb) { \
    for (EntityID::id_type i = 0; i < active_entities.size(); ++i) { \
        if (!BIT_AT_ARR(entity_tracker, active_entities[i])) continue; \
        Entity &ent = entities[active_entities[i]]; \
        SERVER_ONLY(if (ent.pending_delete) continue;) \
        if (ent.has_component(k##name)) cb(this, ent); \
    } \
}
PERCOMPONENT
#undef COMPONENT