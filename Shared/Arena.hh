#pragma once

#include <Shared/StaticData.hh>

#include <string>
#include <cstdint>

SERVER_ONLY(class Writer;)
CLIENT_ONLY(class Reader;)

SERVER_ONLY(typedef float Float;)
CLIENT_ONLY(typedef LerpFloat Float;)

#define FIELDS_Arena \
    SINGLE(player_count, uint32_t) \
    MULTIPLE(scores, Float, LEADERBOARD_SIZE) \
    MULTIPLE(names, std::string, LEADERBOARD_SIZE) \
    MULTIPLE(colors, uint8_t, LEADERBOARD_SIZE)

class Arena {
    enum Fields {
        #define SINGLE(name, type) k##name,
        #define MULTIPLE(name, type, amt) k##name,
        FIELDS_Arena
        #undef SINGLE
        #undef MULTIPLE
        kFieldCount
    };
    uint8_t state[div_round_up(kFieldCount, 8)];
    #define SINGLE(name, type);
    #define MULTIPLE(name, type, amt) uint8_t state_per_##name[div_round_up(amt, 8)];
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
public:
    #define SINGLE(name, type) type name;
    #define MULTIPLE(name, type, count) type name[count];
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE

    Arena();
    void init();
    void reset_protocol();
#ifdef SERVERSIDE
    void write(Writer *, uint8_t);
    #define SINGLE(name, type) void set_##name(type const);
    #define MULTIPLE(name, type, amt) void set_##name(uint32_t, type const);
    FIELDS_Arena
    #undef SINGLE
    #undef MULTIPLE
#else
    void read(Reader *, uint8_t);
#endif
};