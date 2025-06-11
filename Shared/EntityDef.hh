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
    COMPONENT(Web) \
    COMPONENT(Score) \
    COMPONENT(Name)

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
FIELDS_Score \
FIELDS_Name

#define FIELDS_Physics \
SINGLE(Physics, x, Float) \
SINGLE(Physics, y, Float) \
SINGLE(Physics, radius, Float) \
SINGLE(Physics, angle, Float) \
SINGLE(Physics, deletion_tick, uint8)

#define FIELDS_Camera \
SINGLE(Camera, player, entid) \
SINGLE(Camera, respawn_level, uint8) \
MULTIPLE(Camera, inventory, uint8, 2 * MAX_SLOT_COUNT) \
SINGLE(Camera, killed_by, string) \
SINGLE(Camera, camera_x, Float) \
SINGLE(Camera, camera_y, Float) \
SINGLE(Camera, fov, Float) 

#define FIELDS_Relations \
SINGLE(Relations, team, entid) \
SINGLE(Relations, parent, entid)

#define FIELDS_Flower \
SINGLE(Flower, eye_angle, float) \
SINGLE(Flower, overlevel_timer, float) \
SINGLE(Flower, loadout_count, uint32) \
SINGLE(Flower, face_flags, uint8) \
MULTIPLE(Flower, loadout_ids, uint8, 2 * MAX_SLOT_COUNT) \
MULTIPLE(Flower, loadout_reloads, uint8, MAX_SLOT_COUNT)

#define FIELDS_Petal \
SINGLE(Petal, petal_id, uint8)

#define FIELDS_Health \
SINGLE(Health, health_ratio, Float) \
SINGLE(Health, damaged, uint8)

#define FIELDS_Mob \
SINGLE(Mob, mob_id, uint8)

#define FIELDS_Drop \
SINGLE(Drop, drop_id, uint8)

#define FIELDS_Segmented \
SINGLE(Segmented, is_tail, uint8)

#define FIELDS_Web ;

#define FIELDS_Score \
SINGLE(Score, score, uint32)

#define FIELDS_Name \
SINGLE(Name, name, string) \
SINGLE(Name, nametag_visible, uint8)

#ifdef SERVERSIDE
#define PER_EXTRA_FIELD \
    SINGLE(velocity, Vector, .set(0,0)) \
    SINGLE(collision_velocity, Vector, .set(0,0)) \
    SINGLE(acceleration, Vector, .set(0,0)) \
    SINGLE(friction, float, =0) \
    SINGLE(mass, float, =1) \
    SINGLE(speed_ratio, float, =1) \
    SINGLE(lifetime, uint32_t, =0) \
    SINGLE(zone, uint32_t, =0) \
    \
    MULTIPLE(loadout, LoadoutSlot, MAX_SLOT_COUNT, .reset()) \
    SINGLE(heading_angle, float, =0) \
    SINGLE(input, uint8_t, =0) \
    SINGLE(rotation_count, uint8_t, =1) \
    \
    SINGLE(slow_ticks, uint16_t, =0) \
    SINGLE(slow_inflict, uint16_t, =0) \
    SINGLE(immunity_ticks, uint16_t, =0) \
    SINGLE(dandy_ticks, uint16_t, =0) \
    SINGLE(poison_ticks, uint16_t, =0) \
    SINGLE(poison_inflicted, float, =0) \
    SINGLE(poison_dealer, EntityID, =NULL_ENTITY) \
    SINGLE(poison_damage, PoisonDamage, ={}) \
    SINGLE(health, float, =0) \
    SINGLE(max_health, float, =0) \
    SINGLE(damage, float, =0) \
    SINGLE(armor, float, =0) \
    SINGLE(poison_armor, float, =0) \
    SINGLE(damage_reflection, float, =0) \
    SINGLE(last_damaged_by, EntityID, =NULL_ENTITY) \
    \
    SINGLE(owner, EntityID, =NULL_ENTITY) \
    SINGLE(base_entity, EntityID, =NULL_ENTITY) \
    SINGLE(target, EntityID, =NULL_ENTITY) \
    SINGLE(seg_head, EntityID, =NULL_ENTITY) \
    SINGLE(detection_radius, float, =0) \
    SINGLE(ai_tick, uint16_t, =0) \
    SINGLE(ai_state, uint8_t, =0) \
    \
    SINGLE(flags, uint8_t, =0) \
    SINGLE(despawn_tick, uint16_t, =0) \
    SINGLE(secondary_reload, uint16_t, =0) \
    SINGLE(deleted_petals, CIRCARR, ={})

#else
#define PER_EXTRA_FIELD \
    SINGLE(last_damaged_time, double, =0) \
    SINGLE(healthbar_lag, float, =0) \
    SINGLE(healthbar_opacity, float, =0) \
    SINGLE(deletion_animation, float, =0) \
    SINGLE(eye_x, float, =3) \
    SINGLE(eye_y, float, =0) \
    SINGLE(mouth, float, =15) \
    SINGLE(animation, float, =0) \
    SINGLE(damage_flash, float, =0)
#endif

namespace EntityFlags {
    enum {
        IsDespawning = 1 << 0,
        NoFriendlyCollision = 1 << 1,
        DieOnParentDeath = 1 << 2,
        SpawnedFromZone = 1 << 3
    };
};

class EntityID {
public:
    typedef uint16_t hash_type;
    typedef uint16_t id_type;
    hash_type hash;
    id_type id;
    EntityID();
    EntityID(id_type, hash_type);
    bool null() const;
    void operator=(EntityID const &);
    void print();
};

bool operator<(const EntityID &, const EntityID &);
bool operator==(const EntityID &, const EntityID &);

extern EntityID NULL_ENTITY;

#ifdef SERVER_ONLY
class LoadoutPetal {
public:
    uint32_t reload;
    EntityID ent_id;
};

class LoadoutSlot {
public:
    uint8_t id;
    uint8_t already_spawned;
    LoadoutPetal petals[5];
    LoadoutSlot();
    void reset();
};
#endif
