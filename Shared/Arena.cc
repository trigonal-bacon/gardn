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
void Arena::write(Writer *writer, uint8_t create) {
    #define SINGLE(name, type) writer->write_##type(name);
    #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) writer->write_##type(name[i]);
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}
#else
void Arena::read(Reader *reader) {
    #define SINGLE(name, type) reader->read_##type(name);
    #define MULTIPLE(name, type, count) for (uint32_t i = 0; i < count; ++i) reader->read_##type(name[i]);
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
}
#endif