#include <Shared/Entity.hh>

#include <Shared/Binary.hh>
#include <Shared/StaticData.hh>

#include <Shared/Binary.hh>

Entity::Entity() { init(); }

void Entity::init() {
  components = 0;
  pending_delete = 0;
  lifetime = 0;
#ifdef USE_UPTR_ENTITY
#define COMPONENT(component) component = nullptr;
  PERCOMPONENT
#undef COMPONENT
#else
#define SINGLE(component, name, type) name = {};
#define MULTIPLE(component, name, type, count) name = {};
  PERFIELD
#undef SINGLE
#undef MULTIPLE
#endif

#define SINGLE(name, type, reset) name reset;
#define MULTIPLE(name, type, amt, reset)                                       \
  for (uint32_t i = 0; i < amt; ++i) {                                         \
    name[i] reset;                                                             \
  }
  PER_EXTRA_FIELD
#undef SINGLE
#undef MULTIPLE
  reset_protocol();
}

void Entity::reset_protocol() {
  state.reset();
  vector_state.reset();
}

#ifdef USE_UPTR_ENTITY
#define COMPONENT(component)                                                   \
  template <> void Entity::add_component<k##component>() {                     \
    DEBUG_ONLY(assert(!has_component<k##component>());)                        \
    Bits::set(components, k##component);                                       \
    component = std::make_unique<Component_##component>();                     \
  }                                                                            \
  template <> uint8_t Entity::has_component<k##component>() const {            \
    return Bits::at(components, k##component);                                 \
  }
PERCOMPONENT
#undef COMPONENT

#define SINGLE(component, name, type)                                          \
  const type& Entity::get_##name() const {                                     \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return component->name;                                                    \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  const type& Entity::get_##name##_flat(uint32_t index) const { \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return component->name[index];                                              \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE
#else
#define COMPONENT(component)                                                   \
  template <> void Entity::add_component<k##component>() {                     \
    DEBUG_ONLY(assert(!has_component<k##component>());)                        \
    Bits::set(components, k##component);                                       \
  }                                                                            \
  template <> uint8_t Entity::has_component<k##component>() const {            \
    return Bits::at(components, k##component);                                 \
  }
PERCOMPONENT
#undef COMPONENT

#define SINGLE(component, name, type)                                          \
  const type& Entity::get_##name() const {                                     \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return name;                                                               \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  const type& Entity::get_##name##_flat(uint32_t index) const { \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return name[index];                                                        \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE
#endif

#define SINGLE(component, name, type) ;
#define MULTIPLE(component, name, type, ...) \
  const type& Entity::get_##name(const Shape<__VA_ARGS__>::Indices index) const { \
    return get_##name##_flat(Shape<__VA_ARGS__>::index(index)); \
  } \
  type& Entity::get_##name##_flat_mut(uint32_t i) { \
    return const_cast<type&>(get_##name##_flat(i)); \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE

#define SINGLE(component, name, type)                                          \
  type& Entity::get_##name##_mut() { return const_cast<type&>(get_##name()); }
#define MULTIPLE(component, name, type, ...)                                   \
  type& Entity::get_##name##_mut(const Shape<__VA_ARGS__>::Indices index) { \
    return const_cast<type&>(get_##name(index));                                   \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE

#ifdef SERVERSIDE
#define SINGLE(component, name, type)                                          \
  void Entity::set_##name(const type& v) {                                     \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    if (get_##name() == v)                                                     \
      return;                                                                  \
    get_##name##_mut() = v;                                                    \
    state.set(k##name, 1);                                                     \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  void Entity::set_##name(const Shape<__VA_ARGS__>::Indices index, const type& v) {     \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    if (get_##name(index) == v)                                                \
      return;                                                                  \
    get_##name##_mut(index) = v;                                               \
    state.set(k##name, 1);                                                     \
    vector_state.set(_BIT_OFFSETS[k##name]+Shape<__VA_ARGS__>::index(index), 1); \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE

template <> void Entity::write<true>(Writer& writer) const {
  writer.write<uint32_t>(components);
  writer.write<uint32_t>(lifetime);
#define SINGLE(component, name, type)                                          \
  {                                                                            \
    writer.write<type>(get_##name());                                          \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  {                                                                            \
    for (uint32_t n = 0; n < Shape<__VA_ARGS__>::size; ++n)                    \
      writer.write<type>(get_##name##_flat(n));                                \
  }
#define COMPONENT(name)                                                        \
  if (has_component<k##name>()) {                                              \
    FIELDS_##name                                                              \
  }
  PERCOMPONENT
#undef SINGLE
#undef MULTIPLE
#undef COMPONENT
}

template <> void Entity::write<false>(Writer& writer) const {
#define SINGLE(component, name, type)                                          \
  if (state[k##name]) {                                                        \
    writer.write<uint8_t>(k##name);                                            \
    writer.write<type>(get_##name());                                          \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  if (state[k##name]) {                                                        \
    writer.write<uint8_t>(k##name);                                            \
    for (uint32_t n = 0; n < Shape<__VA_ARGS__>::size; ++n) {                  \
      if (vector_state[_BIT_OFFSETS[k##name]+n]) {                             \
        writer.write<uint8_t>(n);                                              \
        writer.write<type>(get_##name##_flat(n));                              \
      }                                                                        \
    }                                                                          \
    writer.write<uint8_t>(Shape<__VA_ARGS__>::size);                               \
  }
#define COMPONENT(name)                                                        \
  if (has_component<k##name>()) {                                              \
    FIELDS_##name                                                              \
  }
  PERCOMPONENT
#undef SINGLE
#undef MULTIPLE
#undef COMPONENT
  writer.write<uint8_t>(kFieldCount);
}

void Entity::write(Writer& writer, uint8_t create) const {
  if (create)
    write<true>(writer);
  else
    write<false>(writer);
}
#else

template <> void Entity::read<true>(Reader& reader) {
  uint32_t set_components = reader.read<uint32_t>();
#define COMPONENT(component)                                                   \
  if (Bits::at(set_components, k##component))                                  \
    add_component<k##component>();
  PERCOMPONENT
#undef COMPONENT
  lifetime = reader.read<uint32_t>();
#define SINGLE(component, name, type)                                          \
  {                                                                            \
    reader.read<type>(get_##name##_mut());                                     \
    state.set(k##name, 1);                                                     \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  {                                                                            \
    state.set(k##name, 1);                                                     \
    for (uint32_t n = 0; n < Shape<__VA_ARGS__>::size; ++n) {                                       \
      vector_state.set(_BIT_OFFSETS[k##name]+n, 1);                            \
      reader.read<type>(get_##name##_flat_mut(n));                                  \
    }                                                                          \
  }
#define COMPONENT(name)                                                        \
  if (has_component<k##name>()) {                                              \
    FIELDS_##name                                                              \
  }
  PERCOMPONENT
#undef SINGLE
#undef MULTIPLE
#undef COMPONENT
}

template <> void Entity::read<false>(Reader& reader) {
  ++lifetime;
  while (1) {
    switch (reader.read<uint8_t>()) {
    case kFieldCount: {
      return;
    }
#define SINGLE(component, name, type)                                          \
  case k##name: {                                                              \
    reader.read<type>(get_##name##_mut());                                     \
    state.set(k##name, 1);                                                     \
    break;                                                                     \
  }
#define MULTIPLE(component, name, type, ...)                                   \
  case k##name: {                                                              \
    state.set(k##name, 1);                                                     \
    while (1) {                                                                \
      uint8_t index = reader.read<uint8_t>();                                  \
      if (index >= Shape<__VA_ARGS__>::size)                                                        \
        break;                                                                 \
      reader.read<type>(get_##name##_flat_mut(index));                              \
      vector_state.set(_BIT_OFFSETS[k##name]+index, 1);                        \
    }                                                                          \
    break;                                                                     \
  }
      PERFIELD
#undef SINGLE
#undef MULTIPLE
    }
  }
}

void Entity::read(Reader& reader, uint8_t create) {
  if (create)
    read<true>(reader);
  else
    read<false>(reader);
}

#define SINGLE(component, name, type)                                          \
  uint8_t Entity::get_state_##name() const {                                   \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return state[k##name];                                                     \
  }

#define MULTIPLE(component, name, type, ...)                                   \
  uint8_t Entity::get_state_##name(const Shape<__VA_ARGS__>::Indices index) const {                         \
    DEBUG_ONLY(assert(has_component<k##component>());)                         \
    return vector_state[_BIT_OFFSETS[k##name]+Shape<__VA_ARGS__>::index(index)];                              \
  }
PERFIELD
#undef SINGLE
#undef MULTIPLE
#endif