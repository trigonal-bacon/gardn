#include <Shared/Arena.hh>

#include <Shared/Binary.hh>

#include <Helpers/Bits.hh>
#include <Helpers/Macros.hh>

Arena::Arena() { init(); }

void Arena::init() {
#define SINGLE(name, type) name = {};
#define MULTIPLE(name, type, ...) name = {};
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
}

void Arena::reset_protocol() {
  state.reset();
#define SINGLE(name, type) ;
#define MULTIPLE(name, type, amt) state_per_##name.reset();
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
}

#define SINGLE(name, type)                                                     \
  const type& Arena::get_##name() const {                                      \
    return name;                                                               \
  }                                                                            \
  type& Arena::get_##name##_mut() {                                            \
    return const_cast<type&>(get_##name());                                    \
  }
#define MULTIPLE(name, type, ...)                                              \
  const type& Arena::get_##name(const Shape<__VA_ARGS__>::Indices index) const { \
    uint32_t i = Shape<__VA_ARGS__>::index(index);                             \
    return name[i];                                                            \
  }                                                                            \
  type& Arena::get_##name##_mut(const Shape<__VA_ARGS__>::Indices index) {     \
    return const_cast<type&>(get_##name(index));                               \
  }
  FIELDS_Arena
#undef SINGLE
#undef MULTIPLE

#ifdef SERVERSIDE
#define SINGLE(name, type)                                                     \
  void Arena::set_##name(const type& v) {                                      \
    if (name == v)                                                             \
      return;                                                                  \
    name = v;                                                                  \
    state.set(k##name, 1);                                                     \
  }
#define MULTIPLE(name, type, ...)                                              \
  void Arena::set_##name(const Shape<__VA_ARGS__>::Indices index, const type& v) { \
    uint32_t i = Shape<__VA_ARGS__>::index(index);                             \
    if (name[i] == v)                                                          \
      return;                                                                  \
    name[i] = v;                                                               \
    state.set(k##name, 1);                                                     \
    state_per_##name.set(i, 1);                                                \
  }
FIELDS_Arena
#undef SINGLE
#undef MULTIPLE

void Arena::write(Writer& writer, uint8_t create) {
  if (create) {
#define SINGLE(name, type) writer.write<type>(name);
#define MULTIPLE(name, type, ...)                                            \
  for (uint32_t i = 0; i < Shape<__VA_ARGS__>::size; ++i)                    \
    writer.write<type>(name[i]);
    FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
  } else {
#define SINGLE(name, type)                                                    \
  if (state[k##name]) {                                                       \
    writer.write<uint8_t>(k##name);                                           \
    writer.write<type>(name);                                                 \
  }
#define MULTIPLE(name, type, ...)                                             \
  if (state[k##name]) {                                                       \
    writer.write<uint8_t>(k##name);                                           \
    for (uint32_t n = 0; n < Shape<__VA_ARGS__>::size; ++n)                   \
      if (state_per_##name[n]) {                                              \
        writer.write<uint32_t>(n);                                             \
        writer.write<type>(name[n]);                                          \
      }                                                                       \
    writer.write<uint32_t>(Shape<__VA_ARGS__>::size);                          \
  }
    FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
        writer.write<uint8_t>(kFieldCount);
  }
}
#else
void Arena::read(Reader& reader, uint8_t create) {
  if (create) {
#define SINGLE(name, type) reader.read<type>(name);
#define MULTIPLE(name, type, ...)                                              \
  for (uint32_t i = 0; i < Shape<__VA_ARGS__>::size; ++i)                      \
    reader.read<type>(name[i]);
    FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
  } else {
    while (1) {
      switch (reader.read<uint8_t>()) {
      case kFieldCount: {
        return;
      }
#define SINGLE(name, type)                                                     \
  case k##name: {                                                              \
    reader.read<type>(name);                                                  \
    state.set(k##name, 1);                                                     \
    break;                                                                     \
  }
#define MULTIPLE(name, type, ...)                                              \
  case k##name: {                                                              \
    state.set(k##name, 1);                                                     \
    while (1) {                                                                \
      uint32_t index = reader.read<uint32_t>();                                \
      if (index >= Shape<__VA_ARGS__>::size)                                   \
        break;                                                                 \
      reader.read<type>(name[index]);                                          \
      state_per_##name.set(index, 1);                                          \
    }                                                                          \
    break;                                                                     \
  }
        FIELDS_Arena
#undef SINGLE
#undef MULTIPLE
      }
    }
  }
}
#endif
