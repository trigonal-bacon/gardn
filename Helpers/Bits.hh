#pragma once

#include <cstdint>

namespace Bits {
constexpr uint32_t count(uint32_t v) { return 32 - __builtin_clz(v - 1); };
constexpr uint32_t fill(uint32_t v) { return (1 << v) - 1; };
template <typename T> constexpr T at(const T& val, uint32_t bit) {
  return (val >> bit) & 1;
};
template <typename T> constexpr void set(T& val, uint32_t bit) {
  val |= ((T)1 << bit);
};
template <typename T> constexpr void unset(T& val, uint32_t bit) {
  val &= ~((T)1 << bit);
};

constexpr uint8_t at_arr(const uint8_t* arr, uint32_t bit) {
  return at(arr[bit >> 3], bit & 7);
};
constexpr void set_arr(uint8_t* arr, uint32_t bit) {
  set(arr[bit >> 3], bit & 7);
};
constexpr void unset_arr(uint8_t* arr, uint32_t bit) {
  unset(arr[bit >> 3], bit & 7);
};

template<uint32_t k>
struct UintType {
  using T = uint64_t;
};

template<> struct UintType<0> {
  using T = uint8_t;
};

template<> struct UintType<1> {
  using T = uint8_t;
};

template<> struct UintType<2> {
  using T = uint16_t;
};

template<> struct UintType<3> {
  using T = uint32_t;
};

template<> struct UintType<4> {
  using T = uint32_t;
};

template<uint32_t k>
using Uint_t = UintType<(k + 7u) / 8u>::T;

} // namespace Bits

class PersistentFlag {
  uint8_t value : 1;
  uint8_t preserved : 1;

public:
  PersistentFlag() : value(0), preserved(0) {};
  void set(uint8_t v) {
    value = v;
    preserved |= v;
  }
  operator uint8_t() const { return preserved; };
  void clear() { preserved = value; };
};
