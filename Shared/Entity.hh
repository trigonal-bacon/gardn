#pragma once

#include <Shared/Binary.hh>
#include <Shared/EntityDef.hh>
#include <Shared/Helpers.hh>
#include <Shared/Vector.hh>

#include <cstdint>

typedef EntityID entid;
typedef uint8_t uint8;
typedef uint32_t uint32;
typedef int32_t int32;
typedef std::string string;
#define CIRCARR CircularArray<PetalID::T, 8>

enum Components {
    #define COMPONENT(name) k##name,
    PERCOMPONENT
    #undef COMPONENT
    kComponentCount
};

enum Fields {
    #define SINGLE(component, name, type) k##name,
    #define MULTIPLE(component, name, type, amt) k##name,
    PERFIELD
    #undef SINGLE
    #undef MULTIPLE
    kFieldCount
};

class Entity {
public:
    Entity();
    void init();
    void reset_protocol();
    uint32_t components;
    EntityID id;
    uint8_t pending_delete;
#define SINGLE(component, name, type) type name;
#define MULTIPLE(component, name, type, amt) type name[amt];
PERFIELD
#undef SINGLE
#undef MULTIPLE
#define SINGLE(component, name, type) uint8_t state_##name;
#define MULTIPLE(component, name, type, amt) uint8_t state_##name; uint8_t state_per_##name[amt];
PERFIELD
#undef SINGLE
#undef MULTIPLE
void add_component(uint32_t);
uint8_t has_component(uint32_t) const;
#define SINGLE(name, type, reset) type name;
#define MULTIPLE(name, type, amt, reset) type name[amt];
PER_EXTRA_FIELD
#undef SINGLE
#undef MULTIPLE
#ifdef SERVERSIDE
    //make this an entityfunction
    void set_despawn_tick(uint16_t);
    void write(Writer *, uint8_t);
    #define SINGLE(component, name, type) void set_##name(type const);
    #define MULTIPLE(component, name, type, amt) void set_##name(uint32_t, type const);
    PERFIELD
    #undef SINGLE
    #undef MULTIPLE
#else
    void read(Reader *);
#endif
};