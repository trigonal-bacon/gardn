#pragma once

#include <Shared/Binary.hh>
#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

#include <string>
#include <stdint.h>

typedef uint8_t uint8;
typedef uint32_t uint32;
typedef int32_t int32;
typedef std::string string;

#define FIELDS_Arena \
    SINGLE(player_count, uint32) \
    MULTIPLE(scores, Float, LEADERBOARD_SIZE) \
    MULTIPLE(names, string, LEADERBOARD_SIZE)

class Arena {
public:
    enum Fields {
        #define SINGLE(name, type) k##name,
        #define MULTIPLE(name, type, amt) k##name,
        FIELDS_Arena
        #undef SINGLE
        #undef MULTIPLE
        kFieldCount
    };
    uint8_t protocol_state;
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
    void read(Reader *);
#endif
};