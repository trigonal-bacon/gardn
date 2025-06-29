#pragma once

#include <Shared/Arena.hh>
#include <Shared/Entity.hh>
#include <Shared/Helpers.hh>

#ifdef SERVERSIDE
#include <Server/SpatialHash.hh>
#endif

#include <functional>
#include <string>

class Simulation {
public:
    SERVER_ONLY(SpatialHash spatial_hash;)
    Arena arena_info;
    uint8_t entity_tracker[ENTITY_CAP] = {0};
    EntityID::hash_type hash_tracker[ENTITY_CAP] = {0};
    Entity entities[ENTITY_CAP];
    StaticArray<EntityID, ENTITY_CAP> active_entities;
    StaticArray<EntityID, ENTITY_CAP> pending_delete;
    Simulation();
    void reset();
    Entity &alloc_ent();
    void _delete_ent(EntityID const &); //DANGEROUS
    void force_alloc_ent(EntityID const &);
    void request_delete(EntityID const &);
    Entity &get_ent(EntityID const &);
    uint8_t ent_exists(EntityID const &) const;
    uint8_t ent_alive(EntityID const &) const;
    void pre_tick();
    void tick();
    void post_tick();

    template <uint8_t T>
    void for_each(std::function<void (Simulation *, Entity &)>);
#ifdef SERVERSIDE
    //Entity &alloc_mob(uint8_t);
    //Entity &alloc_player(Entity &);
    //Entity &alloc_petal(uint8_t);
#else
    void tick_lerp(double);
#endif
};