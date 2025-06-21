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
    for (uint32_t n = 0; n < div_round_up(kFieldCount, 8); ++n) state[n] = 0;
    #define SINGLE(name, type);
    #define MULTIPLE(name, type, amt); for (uint32_t n = 0; n < div_round_up(amt, 8); ++n) { state_per_##name[n] = 0; }
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}

#ifdef SERVERSIDE
#define SINGLE(name, type) \
void Arena::set_##name(type const v) { \
    if (name == v) return; \
    name = v; \
    BIT_SET_ARR(state, k##name); \
}
#define MULTIPLE(name, type, amt) \
void Arena::set_##name(uint32_t i, type const v) { \
    if (name[i] == v) return; \
    name[i] = v; \
    BIT_SET_ARR(state, k##name); \
    BIT_SET_ARR(state_per_##name, i); \
}
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE

void Arena::write(Writer *writer, uint8_t create) {
    if (create) {
        #define SINGLE(name, type) writer->write<type>(name);
        #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) writer->write<type>(name[i]);
        FIELDS_Arena
        #undef SINGLE
        #undef MULTIPLE
    } else {
#define SINGLE(name, type) if(BIT_AT_ARR(state, k##name)) { writer->write<uint8_t>(k##name); writer->write<type>(name); }
#define MULTIPLE(name, type, amt) if(BIT_AT_ARR(state, k##name)) { \
    writer->write<uint8_t>(k##name); \
    for (uint32_t n = 0; n < amt; ++n) \
        if (BIT_AT_ARR(state_per_##name, n)) { writer->write<uint8_t>(n); writer->write<type>(name[n]); } \
        writer->write<uint8_t>(amt); \
    }
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
    writer->write<uint8_t>(kFieldCount);
    }
}
#else
void Arena::read(Reader *reader, uint8_t create) {
    if (create) {
        #define SINGLE(name, type) reader->read<type>(name);
        #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) reader->read<type>(name[i]);
        FIELDS_Arena
        #undef SINGLE
        #undef MULTIPLE
    } else {
        while(1) {
            switch(reader->read<uint8_t>()) {
                case kFieldCount: { return; }
                #define SINGLE(name, type) case k##name: { \
                    reader->read<type>(name); \
                    BIT_SET_ARR(state, k##name); \
                    break; \
                }
                #define MULTIPLE(name, type, amt) case k##name: { \
                    BIT_SET_ARR(state, k##name); \
                    while(1) { \
                        uint8_t index = reader->read<uint8_t>(); \
                        if (index >= amt) break; \
                        reader->read<type>(name[index]); \
                        BIT_SET_ARR(state_per_##name, index); \
                    } \
                    break; \
                }
                FIELDS_Arena
                #undef SINGLE  
                #undef MULTIPLE      
            }
        }
    }
}
#endif