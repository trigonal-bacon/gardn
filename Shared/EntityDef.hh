#pragma once

#include <Shared/StaticDefinitions.hh>

#include <cstdint>

typedef uint16_t game_tick_t;

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
SINGLE(Physics, angle, Float)

#define FIELDS_Camera \
SINGLE(Camera, player, EntityID) \
SINGLE(Camera, respawn_level, uint8_t) \
MULTIPLE(Camera, inventory, PetalID::T, 2 * MAX_SLOT_COUNT) \
SINGLE(Camera, killed_by, std::string) \
SINGLE(Camera, camera_x, Float) \
SINGLE(Camera, camera_y, Float) \
SINGLE(Camera, fov, Float) 

#define FIELDS_Relations \
SINGLE(Relations, team, EntityID) \
SINGLE(Relations, parent, EntityID) \
SINGLE(Relations, color, uint8_t)

#define FIELDS_Flower \
SINGLE(Flower, overlevel_timer, float) \
SINGLE(Flower, loadout_count, uint8_t) \
SINGLE(Flower, face_flags, uint8_t) \
SINGLE(Flower, equip_flags, uint8_t) \
MULTIPLE(Flower, loadout_ids, PetalID::T, 2 * MAX_SLOT_COUNT) \
MULTIPLE(Flower, loadout_reloads, uint8_t, MAX_SLOT_COUNT)

#define FIELDS_Petal \
SINGLE(Petal, petal_id, PetalID::T)

#define FIELDS_Health \
SINGLE(Health, health_ratio, Float) \
SINGLE(Health, damaged, StickyFlag)

#define FIELDS_Mob \
SINGLE(Mob, mob_id, MobID::T)

#define FIELDS_Drop \
SINGLE(Drop, drop_id, PetalID::T)

#define FIELDS_Segmented

#define FIELDS_Web

#define FIELDS_Score \
SINGLE(Score, score, uint32_t)

#define FIELDS_Name \
SINGLE(Name, name, std::string) \
SINGLE(Name, nametag_visible, uint8_t)

#ifdef SERVERSIDE
#define PER_EXTRA_FIELD \
    SINGLE(velocity, Vector, .set(0,0)) \
    SINGLE(collision_velocity, Vector, .set(0,0)) \
    SINGLE(acceleration, Vector, .set(0,0)) \
    SINGLE(friction, float, =0) \
    SINGLE(mass, float, =1) \
    SINGLE(speed_ratio, float, =1) \
    \
    MULTIPLE(loadout, LoadoutSlot, MAX_SLOT_COUNT, .reset()) \
    SINGLE(heading_angle, float, =0) \
    SINGLE(input, uint8_t, =0) \
    SINGLE(player_count, uint32_t, =0) \
    \
    SINGLE(slow_ticks, game_tick_t, =0) \
    SINGLE(slow_inflict, game_tick_t, =0) \
    SINGLE(immunity_ticks, game_tick_t, =0) \
    SINGLE(dandy_ticks, game_tick_t, =0) \
    SINGLE(poison_ticks, game_tick_t, =0) \
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
    SINGLE(score_reward, uint32_t, =0) \
    \
    SINGLE(base_entity, EntityID, =NULL_ENTITY) \
    SINGLE(target, EntityID, =NULL_ENTITY) \
    SINGLE(seg_head, EntityID, =NULL_ENTITY) \
    SINGLE(detection_radius, float, =0) \
    SINGLE(ai_tick, game_tick_t, =0) \
    SINGLE(ai_state, uint8_t, =0) \
    \
    SINGLE(zone, uint8_t, =0) \
    SINGLE(flags, uint8_t, =0) \
    SINGLE(deletion_tick, uint8_t, =0) \
    SINGLE(despawn_tick, game_tick_t, =0) \
    SINGLE(secondary_reload, game_tick_t, =0) \
    SINGLE(deleted_petals, circ_arr_t, ={})
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

class EntityID {
public:
    typedef uint8_t hash_type;
    typedef uint16_t id_type;
    id_type id;
    hash_type hash;
    EntityID();
    EntityID(id_type, hash_type);
    static uint32_t make_hash(EntityID const);
    static bool equal_to(EntityID const, EntityID const);
    bool null() const;
};

bool operator<(EntityID const, EntityID const);
bool operator==(EntityID const, EntityID const);

inline const EntityID NULL_ENTITY;

#ifdef SERVER_ONLY
class Simulation;

struct LoadoutPetal {
    EntityID ent_id;
    game_tick_t reload;
};

class LoadoutSlot {
    PetalID::T id;
public:
    uint8_t already_spawned;
    LoadoutPetal petals[MAX_PETALS_IN_CLUMP];    
    LoadoutSlot();
    void reset();
    void update_id(Simulation *, PetalID::T);
    void force_reload();
    PetalID::T get_petal_id() const;
    uint32_t size() const;
};
#endif