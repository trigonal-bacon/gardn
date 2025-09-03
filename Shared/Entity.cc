#include <Shared/Entity.hh>

#include <Shared/Binary.hh>
#include <Shared/StaticData.hh>

#include <Shared/Binary.hh>

Entity::Entity() {
    init();
}

void Entity::init() {
    components = 0;
    pending_delete = 0;
    lifetime = 0;
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
    for (uint32_t n = 0; n < div_round_up(kFieldCount, 8); ++n) state[n] = 0;
    #define SINGLE(component, name, type);
    #define MULTIPLE(component, name, type, amt); for (uint32_t n = 0; n < div_round_up(amt, 8); ++n) { state_per_##name[n] = 0; }
    PERFIELD
    #undef SINGLE
    #undef MULTIPLE
}

void Entity::add_component(uint32_t comp) {
    DEBUG_ONLY(assert(!has_component(comp));)
    BitMath::set(components, comp);
}

uint8_t Entity::has_component(uint32_t comp) const {
    return BitMath::at(components, comp);
}

#define SINGLE(component, name, type) \
type const &Entity::get_##name() const { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    return name; \
}
#define MULTIPLE(component, name, type, amt) \
type const &Entity::get_##name(uint32_t i) const { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    return name[i]; \
}
PERFIELD
#undef SINGLE
#undef MULTIPLE

#ifdef SERVERSIDE
#define SINGLE(component, name, type) \
void Entity::set_##name(type const &v) { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    if (name == v) return; \
    name = v; \
    BitMath::set_arr(state, k##name); \
}
#define MULTIPLE(component, name, type, amt) \
void Entity::set_##name(uint32_t i, type const &v) { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    if (name[i] == v) return; \
    name[i] = v; \
    BitMath::set_arr(state, k##name); \
    BitMath::set_arr(state_per_##name, i); \
}
PERFIELD
#undef SINGLE
#undef MULTIPLE

template<>
void Entity::write<true>(Writer *writer) {
    writer->write<uint32_t>(components);
    writer->write<uint32_t>(lifetime);
    #define SINGLE(component, name, type) { writer->write<type>(name); }
    #define MULTIPLE(component, name, type, amt) { \
        for (uint32_t n = 0; n < amt; ++n) \
            writer->write<type>(name[n]); \
    }
    #define COMPONENT(name) if (has_component(k##name)) { FIELDS_##name }
    PERCOMPONENT
    #undef SINGLE
    #undef MULTIPLE
    #undef COMPONENT
}

template<>
void Entity::write<false>(Writer *writer) {
    #define SINGLE(component, name, type) \
        if(BitMath::at_arr(state, k##name)) { \
            writer->write<uint8_t>(k##name); \
            writer->write<type>(name); \
    }
    #define MULTIPLE(component, name, type, amt) \
        if(BitMath::at_arr(state, k##name)) { \
            writer->write<uint8_t>(k##name); \
            for (uint32_t n = 0; n < amt; ++n) { \
                if (BitMath::at_arr(state_per_##name, n)) { \
                    writer->write<uint8_t>(n); \
                    writer->write<type>(name[n]); \
                } \
            } \
            writer->write<uint8_t>(amt); \
        }
    #define COMPONENT(name) if (has_component(k##name)) { FIELDS_##name }
    PERCOMPONENT
    #undef SINGLE
    #undef MULTIPLE
    #undef COMPONENT
    writer->write<uint8_t>(kFieldCount);
}

void Entity::write(Writer *writer, uint8_t create) {
    if (create) write<true>(writer);
    else write<false>(writer);
}
#else

template<>
void Entity::read<true>(Reader *reader) {
    components = reader->read<uint32_t>();
    lifetime = reader->read<uint32_t>();
    #define SINGLE(component, name, type) { reader->read<type>(name); BitMath::set_arr(state, k##name); }
    #define MULTIPLE(component, name, type, amt) { \
        BitMath::set_arr(state, k##name); \
        for (uint32_t n = 0; n < amt; ++n) { \
            BitMath::set_arr(state_per_##name, n); \
            reader->read<type>(name[n]); \
        } \
    }
    #define COMPONENT(name) if (has_component(k##name)) { FIELDS_##name }
    PERCOMPONENT
    #undef SINGLE
    #undef MULTIPLE
    #undef COMPONENT
}

template<>
void Entity::read<false>(Reader *reader) {
    ++lifetime;
    while(1) {
        switch(reader->read<uint8_t>()) {
            case kFieldCount: { return; }
            #define SINGLE(component, name, type) case k##name: { \
                reader->read<type>(name); \
                BitMath::set_arr(state, k##name); \
                break; \
            }
            #define MULTIPLE(component, name, type, amt) case k##name: { \
                BitMath::set_arr(state, k##name); \
                while (1) { \
                    uint8_t index = reader->read<uint8_t>(); \
                    if (index >= amt) break; \
                    reader->read<type>(name[index]); \
                    BitMath::set_arr(state_per_##name, index); \
                } \
                break; \
            }
            PERFIELD
            #undef SINGLE  
            #undef MULTIPLE      
        }
    }
}

void Entity::read(Reader *reader, uint8_t create) {
    if (create) read<true>(reader);
    else read<false>(reader);
}

#define SINGLE(component, name, type) \
uint8_t Entity::get_state_##name() const { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    return BitMath::at_arr(state, k##name); \
}

#define MULTIPLE(component, name, type, amt) \
uint8_t Entity::get_state_##name(uint32_t i) const { \
    DEBUG_ONLY(assert(has_component(k##component));) \
    return BitMath::at_arr(state_per_##name, i); \
}
PERFIELD
#undef SINGLE
#undef MULTIPLE
#endif