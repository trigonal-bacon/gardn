#pragma once

#include <Helpers/Macros.hh>

#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>

template<typename T, uint32_t capacity>
class StaticArray {
    std::array<T, capacity> values;
    uint32_t length;
public:
    constexpr StaticArray() : length(0) {};
    constexpr StaticArray(std::initializer_list<T> elts) : length(0) {
        assert(elts.size() <= capacity);
        for (T x : elts) values[length++] = x;
    };
    constexpr T &operator[](uint32_t at) { return values[at]; };
    constexpr T const &operator[](uint32_t at) const { return values[at]; };
    constexpr uint32_t size() const { return length; };
    constexpr void push(T val) { DEBUG_ONLY(assert(length < capacity); ) values[length++] = val; };
    constexpr T pop() { DEBUG_ONLY(assert(length > 0); ) return values[--length]; };
    constexpr void clear() { length = 0; }
    constexpr T *begin() { return &values[0]; };
    constexpr T *end() { return &values[length]; };
    constexpr T const *begin() const { return &values[0]; };
    constexpr T const *end() const { return &values[length]; };
};

template<typename T, uint32_t max_len>
class CircularArray {
    std::array<T, max_len> values;
    uint32_t start;
    uint32_t length;
public:
    constexpr CircularArray() : start(0), length(0) { static_assert(max_len > 0); };
    constexpr uint32_t size() const { return length; }
    constexpr uint32_t capacity() const { return max_len; }
    constexpr T &operator[](uint32_t idx) {
        DEBUG_ONLY(assert(idx < size());)
        return values[(start+idx)%max_len];
    };
    constexpr T operator[](uint32_t idx) const {
        DEBUG_ONLY(assert(idx < size());)
        return values[(start+idx)%max_len];
    };
    constexpr void push_back(T const &val) {
        values[(start+length)%max_len] = val;
        if (length == max_len)
            start = (start + 1) % max_len;
        else
            ++length;
    };
    constexpr T pop_back() {
        DEBUG_ONLY(assert(size() > 0);)
        --length;
        return values[(start+length)%max_len];
    }
    constexpr void clear() { start = length = 0; };
};