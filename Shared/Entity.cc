#include <Shared/Entity.hh>

#include <Shared/StaticData.hh>

Entity::Entity() {
    init();
}

void Entity::init() {
    components = 0;
    pending_delete = 0;
    #define SINGLE(component, name, type) name = {};
    #define MULTIPLE(component, name, type, amt) for (uint32_t n = 0; n < amt; ++n) { name[n] = {}; }
    PERFIELD
    #undef SINGLE
    #undef MULTIPLE
    #define SINGLE(name, type, reset) name reset;
    #define MULTIPLE(name, type, amt, reset) for (uint32_t i = 0; i < amt; ++i) { name[i] reset; }
    PER_EXTRA_FIELD
    #undef SINGLE
    #undef MULTIPLE
    reset_protocol();
}

void Entity::reset_protocol() {
    #define SINGLE(component, name, type) state_##name = 0;
    #define MULTIPLE(component, name, type, amt) state_##name = 0; for (uint32_t n = 0; n < amt; ++n) { state_per_##name[n] = 0; }
    PERFIELD
    #undef SINGLE
    #undef MULTIPLE
}

void Entity::add_component(uint32_t comp) {
    DEBUG_ONLY(assert(!has_component(comp));)
    components |= 1 << comp;
}
uint8_t Entity::has_component(uint32_t comp) const {
    return (components >> comp) & 1;
}

#ifdef SERVERSIDE
void Entity::set_despawn_tick(uint16_t t) {
    despawn_tick = t;
    flags |= EntityFlags::IsDespawning;
}

#define SINGLE(component, name, type) \
void Entity::set_##name(type const v) { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    if (name == v) return; \
    name = v; \
    state_##name = 1; \
}
#define MULTIPLE(component, name, type, amt) \
void Entity::set_##name(uint32_t i, type const v) { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    if (name[i] == v) return; \
    name[i] = v; \
    state_##name = 1; \
    state_per_##name[i] = 1; \
}

PERFIELD
#undef SINGLE
#undef MULTIPLE
void Entity::write(Writer *writer, uint8_t create) {
    writer->write_uint32(components);
#define SINGLE(component, name, type) if(create || state_##name) { writer->write_uint8(k##name); writer->write_##type(name); }
#define MULTIPLE(component, name, type, amt) if(create || state_##name) { \
    writer->write_uint8(k##name); \
    for (uint32_t n = 0; n < amt; ++n) { \
        if (create || state_per_##name[n]) { writer->write_uint8(n); writer->write_##type(name[n]); } \
        } \
    } \
    writer->write_uint8(255);
#define COMPONENT(name) if (has_component(k##name)) { FIELDS_##name }
PERCOMPONENT
#undef SINGLE
#undef MULTIPLE
    writer->write_uint8(kFieldCount);
}
#else

void Entity::read(Reader *reader) {
    components = reader->read_uint32();
    while(1) {
        switch(reader->read_uint8()) {
            case kFieldCount: { return; }
            #define SINGLE(component, name, type) case k##name: { \
                reader->read_##type(name); \
                state_##name = 1; \
                break; \
            }
            #define MULTIPLE(component, name, type, amt) case k##name: { \
                while(1) { \
                    uint8_t index = reader->read_uint8(); \
                    if (index == 255) break; \
                    reader->read_##type(name[index]); \
                    state_per_##name[index] = 1; \
                } \
                break; \
            }
            PERFIELD
            #undef SINGLE  
            #undef MULTIPLE      
        }
    }
}
#endif