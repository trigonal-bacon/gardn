#pragma once

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
SINGLE(x, float) \
SINGLE(y, float) \
SINGLE(radius, float) \
SINGLE(angle, float)

#define FIELDS_Camera \
SINGLE(camera_x, float) \
SINGLE(camera_y, float) \
SINGLE(fov, float) \
SINGLE(player, entid)

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
    SINGLE(friction, float, =0)
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
