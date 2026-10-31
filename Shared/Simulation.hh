#pragma once

#include <Shared/Arena.hh>
#include <Shared/Entity.hh>

#include <bitset>
#include <functional>
#include <string>

inline const uint32_t ENTITY_CAP = 8192;

template<class Derived>
class SimulationBase {
  friend Derived;
  std::bitset<ENTITY_CAP> entity_tracker;
  std::array<EntityID::hash_type, ENTITY_CAP> hash_tracker;
  std::array<Entity, ENTITY_CAP> entities;
  StaticArray<EntityID::id_type, ENTITY_CAP> active_entities;

public:
  Arena arena_info;
  SimulationBase() {}
  void reset() {
    /*
    active_entities.clear();
    hash_tracker = {0};
    entity_tracker = {0};

    for (EntityID::id_type i = 0; i < ENTITY_CAP; ++i)
      entities[i].init();

    arena_info.init();
    */
    std::destroy_at(static_cast<Derived*>(this));
    std::construct_at(static_cast<Derived*>(this));
  }
  Entity& alloc_ent() {
    for (EntityID::id_type i = 1; i < ENTITY_CAP; ++i) {
      if (entity_tracker[i])
        continue;
      entity_tracker.set(i, 1);
      entities[i].init();
      DEBUG_ONLY(std::cout << "ent_create " << EntityID(i, hash_tracker[i])
                          << "\n";)
      entities[i].id = EntityID(i, hash_tracker[i]);
      return entities[i];
    }
    assert(!"Entity cap reached");
  }
  void _delete_ent(const EntityID id) {
    DEBUG_ONLY(std::cout << "ent_delete " << id << "\n";)
    DEBUG_ONLY(assert(ent_exists(id)));
    entity_tracker.set(id.id, 0);
    hash_tracker[id.id]++;
  } // DANGEROUS
  void request_delete(const EntityID id) {
    DEBUG_ONLY(assert(ent_exists(id)));
    entities[id.id].pending_delete = 1;
  }
  Entity& get_ent(const EntityID id) {
    DEBUG_ONLY(assert(ent_exists(id));)
    return entities[id.id];
  }
  uint8_t ent_exists(const EntityID id) const {
    DEBUG_ONLY(assert(id.id < ENTITY_CAP);)
    return entity_tracker[id.id] && hash_tracker[id.id] == id.hash;
  }
  uint8_t ent_alive(const EntityID id) const {
    return ent_exists(id) && !entities[id.id].pending_delete 
    SERVER_ONLY(&& entities[id.id].deletion_tick == 0);
  }
  void tick() {
    active_entities.clear();
    for (EntityID::id_type i = 1; i < ENTITY_CAP; ++i) {
      if (!entity_tracker[i])
        continue;
      active_entities.push(entities[i].id.id);
    }
    static_cast<Derived&>(*this).on_tick();
  }

  // will only consider active entities from the start of the tick() call
  template<typename F>
  void for_each_entity(F&& cb) {
    for (EntityID::id_type id : active_entities) {
      if (!entity_tracker[id])
        continue;
      Entity& ent = entities[id];
      cb(static_cast<Derived&>(*this), ent);
    }
  }
  template <uint8_t component, typename F> 
  void for_each(F&& cb) {
    for (EntityID::id_type id : active_entities) {
      if (!entity_tracker[id])
        continue;
      Entity& ent = entities[id];
      SERVER_ONLY(if (ent.pending_delete) continue;)
      if (ent.has_component<component>())
        cb(static_cast<Derived&>(*this), ent);
    }
  }
};
