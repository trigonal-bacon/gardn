#pragma once

#include <Shared/Arena.hh>
#include <Shared/Entity.hh>

#ifdef SERVERSIDE
#include <Server/SpatialHash.hh>
#endif

#include <functional>
#include <string>

inline uint32_t const ENTITY_CAP = 8192;

class Simulation {
    std::array<uint8_t, div_round_up(ENTITY_CAP, 8)> entity_tracker;
    std::array<EntityID::hash_type, ENTITY_CAP> hash_tracker;
    std::array<Entity, ENTITY_CAP> entities;
    StaticArray<EntityID::id_type, ENTITY_CAP> active_entities;
public:
    SERVER_ONLY(std::array<uint32_t, PetalID::kNumPetals> petal_count_tracker;)
    SERVER_ONLY(std::array<uint32_t, MAP_DATA.size()> zone_mob_counts;)
    SERVER_ONLY(SpatialHash spatial_hash;)
    Arena arena_info;
    Simulation();
    void reset();
    Entity &alloc_ent();
    void _delete_ent(EntityID const &); //DANGEROUS
    void force_alloc_ent(EntityID const &);
    void request_delete(EntityID const &);
    Entity &get_ent(EntityID const &);
    uint8_t ent_exists(EntityID const &) const;
    uint8_t ent_alive(EntityID const &) const;
    void tick();
    void on_tick();
    void post_tick();

    //will only consider active entities from the start of the tick() call
    void for_each_entity(std::function<void (Simulation *, Entity &)>);
    template <uint8_t>
    void for_each(std::function<void (Simulation *, Entity &)>);
};