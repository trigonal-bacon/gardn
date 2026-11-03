#pragma once

#include <Helpers/Macros.hh>

#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>

template <typename T, uint32_t Capacity> class StaticArray {
  std::array<T, Capacity> values;
  uint32_t length;

public:
  constexpr StaticArray() : length(0) {};
  constexpr StaticArray(std::initializer_list<T> elts) : length(0) {
    assert(elts.size() <= Capacity);
    for (T x : elts)
      values[length++] = x;
  };
  constexpr T& operator[](uint32_t at) { return values[at]; };
  constexpr const T& operator[](uint32_t at) const { return values[at]; };
  constexpr uint32_t size() const { return length; };
  constexpr uint32_t capacity() const { return Capacity; };
  constexpr void push(T val) {
    DEBUG_ONLY(assert(length < Capacity);) values[length++] = val;
  };
  constexpr T pop() {
    DEBUG_ONLY(assert(length > 0);) return values[--length];
  };
  constexpr void clear() { length = 0; }
  constexpr T* begin() { return values.data(); };
  constexpr T* end() { return values.data() + length; };
  constexpr const T* begin() const { return values.data(); };
  constexpr const T* end() const { return values.data() + length; };
};

template <typename T, uint32_t Capacity> class CircularArray {
  std::array<T, Capacity> values;
  uint32_t start;
  uint32_t length;

public:
  constexpr CircularArray() : start(0), length(0) {
    static_assert(Capacity > 0);
  };
  constexpr uint32_t size() const { return length; }
  constexpr uint32_t capacity() const { return Capacity; }
  constexpr T& operator[](uint32_t idx) {
    DEBUG_ONLY(assert(idx < size());)
    return values[(start + idx) % Capacity];
  };
  constexpr T operator[](uint32_t idx) const {
    DEBUG_ONLY(assert(idx < size());)
    return values[(start + idx) % Capacity];
  };
  constexpr void push_back(const T& val) {
    values[(start + length) % Capacity] = val;
    if (length == Capacity)
      start = (start + 1) % Capacity;
    else
      ++length;
  };
  constexpr T pop_back() {
    DEBUG_ONLY(assert(size() > 0);)
    --length;
    return values[(start + length) % Capacity];
  }
  constexpr void clear() { start = length = 0; };
};

template<size_t... dims>
struct Shape {
    static constexpr size_t size = (dims * ... * 1);
    static constexpr size_t rank = sizeof...(dims);

    using Indices = std::array<uint32_t, rank>;

    static constexpr size_t index(const Indices& indices) {
      size_t flat = 0;
      size_t i = 0;

      ((DEBUG_ONLY(assert(indices[i] < dims),) flat = flat * dims + indices[i++]), ...);
      return flat;
    }
};

template<typename T, size_t... dims>
struct NDArray {
    using shape = Shape<dims...>;
    using Indices = shape::Indices;

    std::array<T, shape::size> data;

    constexpr T& operator()(const Indices& indices) {
        return data[shape::index(indices)];
    }

    constexpr const T& operator()(const Indices& indices) const {
        return data[shape::index(indices)];
    }

    constexpr const T& operator[](size_t i) const {
      DEBUG_ONLY(assert(i < shape::size);)
      return data[i];
    }

    constexpr T& operator[](size_t i) {
      DEBUG_ONLY(assert(i < shape::size);)
      return data[i];
    }
};