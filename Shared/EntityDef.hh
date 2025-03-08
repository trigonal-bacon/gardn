#pragma once

#include <Shared/StaticData.hh>

#include <cstdint>

#define PERCOMPONENT \
    COMPONENT(Physics) \
    COMPONENT(Camera) \
    COMPONENT(Relations) \
    COMPONENT(Flower) \
    COMPONENT(Petal) \
    COMPONENT(Health) \
    COMPONENT(Mob) \
    COMPONENT(Drop) \
    COMPONENT(Segmented) \
    COMPONENT(Web)

#define PERFIELD \
FIELDS_Physics \
FIELDS_Camera \
FIELDS_Relations \
FIELDS_Flower \
FIELDS_Petal \
FIELDS_Health \
FIELDS_Mob \
FIELDS_Drop \
FIELDS_Segmented \
FIELDS_Web

#define FIELDS_Physics \
SINGLE(x, Float) \
SINGLE(y, Float) \
SINGLE(radius, Float) \
SINGLE(angle, Float) \
SINGLE(deletion_tick, Float)

#define FIELDS_Camera \
SINGLE(camera_x, Float) \
SINGLE(camera_y, Float) \
SINGLE(fov, Float) \
SINGLE(player, entid) \
SINGLE(loadout_count, uint8) \
MULTIPLE(loadout_ids, uint8, 2 * MAX_SLOT_COUNT) \
MULTIPLE(loadout_reloads, uint8, MAX_SLOT_COUNT)

#define FIELDS_Relations \
SINGLE(team, entid) \
SINGLE(parent, entid)

#define FIELDS_Flower \
SINGLE(eye_angle, float) \
SINGLE(face_flags, uint8)

#define FIELDS_Petal \
SINGLE(petal_id, uint8)

#define FIELDS_Health \
SINGLE(damaged, uint8) \
SINGLE(health_ratio, Float)

#define FIELDS_Mob \
SINGLE(mob_id, uint8)

#define FIELDS_Drop \
SINGLE(drop_id, uint8)

#define FIELDS_Segmented \
SINGLE(is_tail, uint8)

#define FIELDS_Web \
SINGLE(__useless, uint8)

#ifdef SERVERSIDE
#define PER_EXTRA_FIELD \
    SINGLE(velocity, Vector, .set(0,0)) \
    SINGLE(collision_velocity, Vector, .set(0,0)) \
    SINGLE(acceleration, Vector, .set(0,0)) \
    SINGLE(friction, float, =0) \
    SINGLE(mass, float, =1) \
    SINGLE(speed_ratio, float, =1) \
    SINGLE(lifetime, uint32_t, =0) \
    \
    MULTIPLE(loadout, LoadoutSlot, MAX_SLOT_COUNT, .reset()) \
    SINGLE(petal_rotation, float, =0) \
    SINGLE(input, uint8_t, =0) \
    SINGLE(rotation_count, uint8_t, =1) \
    \
    SINGLE(health, float, =0) \
    SINGLE(max_health, float, =0) \
    SINGLE(damage, float, =0) \
    SINGLE(armor, float, =0) \
    SINGLE(immunity_ticks, uint32_t, =0) \
    SINGLE(dandy_ticks, uint32_t, =0) \
    SINGLE(poison_damage, Poison, ={}) \
    SINGLE(poison, Poison, ={}) \
    \
    SINGLE(ai_tick, uint32_t, =0) \
    SINGLE(ai_state, uint8_t, =0) \
    SINGLE(target, EntityID, =NULL_ENTITY) \
    SINGLE(seg_head, EntityID, =NULL_ENTITY) \
    \
    SINGLE(despawn_tick, uint32_t, =0) \
    SINGLE(secondary_reload, uint32_t, =0) \
    SINGLE(owner_slot, LoadoutPetal *, =nullptr)

#else
#define PER_EXTRA_FIELD \
    SINGLE(eye_x, float, =3) \
    SINGLE(eye_y, float, =0) \
    SINGLE(mouth, float, =15) \
    SINGLE(animation, float, =0) \
    SINGLE(damage_flash, float, =0)
#endif

class EntityID {
public:
    uint16_t hash;
    uint16_t id;
    EntityID();
    EntityID(uint16_t, uint16_t);
    bool null() const;
    void operator=(EntityID const &);
    void print();
};

bool operator<(const EntityID &, const EntityID &);
bool operator==(const EntityID &, const EntityID &);

extern EntityID NULL_ENTITY;

class LoadoutPetal {
public:
    uint32_t reload;
    EntityID ent_id;
};

class LoadoutSlot {
public:
    uint8_t id;
    uint8_t already_spawned;
    LoadoutPetal petals[3];
    LoadoutSlot();
    void reset();
};
