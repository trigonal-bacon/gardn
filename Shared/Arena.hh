#pragma once

#include <Shared/StaticData.hh>

#include <Helpers/Array.hh>

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
#define MULTIPLE(name, type, ...) std::bitset<Shape<__VA_ARGS__>::size> state_per_##name;
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
#define SINGLE(name, type) type name;
#define MULTIPLE(name, type, ...) NDArray<type, __VA_ARGS__> name;
    FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
  public :

  Arena();
  void init();
  void reset_protocol();
#define SINGLE(name, type) \
  const type& get_##name() const; \
  type& get_##name##_mut();
#define MULTIPLE(name, type, ...) \
  const type& get_##name(const Shape<__VA_ARGS__>::Indices) const; \
  type& get_##name##_mut(const Shape<__VA_ARGS__>::Indices);
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
#ifdef SERVERSIDE
  void write(Writer&, uint8_t);
#define SINGLE(name, type) void set_##name(const type&);
#define MULTIPLE(name, type, ...) void set_##name(const Shape<__VA_ARGS__>::Indices, const type&);
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
#else
  void read(Reader&, uint8_t);
#endif
};