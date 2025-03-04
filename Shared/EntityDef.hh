#pragma once

#include <Shared/StaticData.hh>

#include <cstdint>

#define PERCOMPONENT \
    COMPONENT(Physics) \
    COMPONENT(Camera) \
    COMPONENT(Relations) \
    COMPONENT(Flower) \
    COMPONENT(Petal)

#define PERFIELD \
FIELDS_Physics \
FIELDS_Camera \
FIELDS_Relations \
FIELDS_Flower \
FIELDS_Petal

#define FIELDS_Physics \
SINGLE(x, Float) \
SINGLE(y, Float) \
SINGLE(radius, Float) \
SINGLE(angle, Float) \
SINGLE(deletion_tick, uint8)

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
SINGLE(face_flags, uint8) \
SINGLE(eye_angle, float)

#define FIELDS_Petal \
SINGLE(petal_id, uint8) \
SINGLE(petal_rarity, uint8)

#ifdef SERVERSIDE
#define PER_EXTRA_FIELD \
    SINGLE(velocity, Vector, .set(0,0)) \
    SINGLE(acceleration, Vector, .set(0,0)) \
    SINGLE(friction, float, =0) \
    MULTIPLE(loadout, LoadoutSlot, MAX_SLOT_COUNT, .reset()) \
    SINGLE(petal_rotation, float, =0) \
    SINGLE(input, uint8_t, =0)
#else
#define PER_EXTRA_FIELD \
    SINGLE(touched, uint8_t, =0) \
    SINGLE(eye_x, float, =3) \
    SINGLE(eye_y, float, =0) \
    SINGLE(mouth, float, =15)
#endif

class EntityId {
public:
    uint16_t hash;
    uint16_t id;
    EntityId();
    EntityId(uint16_t, uint16_t);
    bool null() const;
    void operator=(EntityId const &);
    void print();
};

bool operator<(const EntityId &, const EntityId &);
bool operator==(const EntityId &, const EntityId &);

extern EntityId NULL_ENTITY;

class LoadoutPetal {
public:
    uint32_t reload;
    EntityId ent_id;
};

class LoadoutSlot {
public:
    uint8_t id;
    LoadoutPetal petals[3];
    LoadoutSlot();
    void reset();
};
