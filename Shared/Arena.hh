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
#else
    void read(Reader *);
#endif
};