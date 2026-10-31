#pragma once

#include <Shared/StaticData.hh>

#include <array>
#include <bitset>
#include <cstdint>
#include <string>

SERVER_ONLY(class Writer;)
CLIENT_ONLY(class Reader;)

SERVER_ONLY(using Float = float;)
CLIENT_ONLY(using Float = LerpFloat;)

#define FIELDS_Arena                                                           \
  SINGLE(player_count, uint32_t)                                               \
  MULTIPLE(scores, Float, LEADERBOARD_SIZE)                                    \
  MULTIPLE(names, std::string, LEADERBOARD_SIZE)                               \
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
  std::bitset<kFieldCount> state;
#define SINGLE(name, type) ;
#define MULTIPLE(name, type, amt) std::bitset<amt> state_per_##name;
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
    public :
#define SINGLE(name, type) type name;
#define MULTIPLE(name, type, count) std::array<type, count> name;
    FIELDS_Arena
#undef SINGLE
#undef MULTIPLE

    Arena();
  void init();
  void reset_protocol();
#ifdef SERVERSIDE
  void write(Writer&, uint8_t);
#define SINGLE(name, type) void set_##name(type const);
#define MULTIPLE(name, type, amt) void set_##name(uint32_t, type const);
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
#else
  void read(Reader&, uint8_t);
#endif
};