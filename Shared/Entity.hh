#pragma once

#include <Shared/EntityDef.hh>

#include <Helpers/Array.hh>
#include <Helpers/Bits.hh>
#include <Helpers/Macros.hh>
#include <Helpers/Vector.hh>

#include <array>
#include <bitset>
#include <cstdint>
#include <vector>

typedef CircularArray<PetalID::T, MAX_SLOT_COUNT> circ_arr_t;

SERVER_ONLY(class Writer;)
CLIENT_ONLY(class Reader;)

SERVER_ONLY(typedef uint8_t StickyFlag;)
CLIENT_ONLY(typedef PersistentFlag StickyFlag;)

SERVER_ONLY(typedef float Float;)
CLIENT_ONLY(typedef LerpFloat Float;)

enum Components {
#define COMPONENT(name) k##name,
  PERCOMPONENT
#undef COMPONENT
      kComponentCount
};

class Entity {
  enum Fields {
#define SINGLE(component, name, type) k##name,
#define MULTIPLE(component, name, type, amt) k##name,
    PERFIELD
#undef SINGLE
#undef MULTIPLE
        kFieldCount
  };
  Bits::Uint_t<kComponentCount> components;
#define SINGLE(component, name, type) type name;
#define MULTIPLE(component, name, type, amt) std::array<type, amt> name;
  PERFIELD
#undef SINGLE
#undef MULTIPLE
  std::bitset<kFieldCount> state;
#define SINGLE(component, name, type) ;
#define MULTIPLE(component, name, type, amt) std::bitset<amt> state_per_##name;
  PERFIELD
#undef SINGLE
#undef MULTIPLE
public:
  Entity();
  void init();
  void reset_protocol();
  Entity(const Entity&) = delete;
  Entity& operator=(const Entity&) = delete;
  Entity& operator=(Entity&&) = delete;
  uint32_t lifetime;
  EntityID id;
  uint8_t pending_delete;
  void add_component(uint32_t);
  uint8_t has_component(uint32_t) const;

#define SINGLE(component, name, type) const type& get_##name() const;
#define MULTIPLE(component, name, type, amt)                                   \
  const type& get_##name(uint32_t) const;
  PERFIELD
#undef SINGLE
#undef MULTIPLE

#define SINGLE(name, type, reset) type name;
#define MULTIPLE(name, type, amt, reset) std::array<type, amt> name;
  PER_EXTRA_FIELD
#undef SINGLE
#undef MULTIPLE

#ifdef SERVERSIDE
  void write(Writer&, uint8_t) const;

  template <bool> void write(Writer&) const;
#define SINGLE(component, name, type) void set_##name(const type&);
#define MULTIPLE(component, name, type, amt)                                   \
  void set_##name(uint32_t, const type&);
  PERFIELD
#undef SINGLE
#undef MULTIPLE
#else
  void tick_lerp(float);
  void read(Reader&, uint8_t);

  template <bool> void read(Reader&);

#define SINGLE(component, name, type) uint8_t get_state_##name() const;
#define MULTIPLE(component, name, type, amt)                                   \
  uint8_t get_state_##name(uint32_t) const;
  PERFIELD
#undef SINGLE
#undef MULTIPLE
#endif
};