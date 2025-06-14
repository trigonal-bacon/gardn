#include <Shared/Arena.hh>


Arena::Arena() {
    init();
}

void Arena::init() {
    #define SINGLE(name, type) name = {};
    #define MULTIPLE(name, type, count) for (uint32_t i = 0; i > count; ++i) name[i] = {};
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}

void Arena::reset_protocol() {
    protocol_state = 0;
}

#ifdef SERVERSIDE
#define SINGLE(name, type) \
void Arena::set_##name(type const v) { \
    if (name == v) return; \
    name = v; \
}
#define MULTIPLE(name, type, amt) \
void Arena::set_##name(uint32_t i, type const v) { \
    if (name[i] == v) return; \
    name[i] = v; \
}
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE

void Arena::write(Writer *writer, uint8_t create) {
    #define SINGLE(name, type) writer->write<type>(name);
    #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) writer->write<type>(name[i]);
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}
#else
void Arena::read(Reader *reader) {
    #define SINGLE(name, type) reader->read<type>(name);
    #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) reader->read<type>(name[i]);
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}
#endif