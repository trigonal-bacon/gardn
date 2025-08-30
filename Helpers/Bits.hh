#pragma once

#include <cstdint>

namespace BitMath {
    constexpr uint32_t count(uint32_t v) {return 32 - __builtin_clz(v - 1); };
    constexpr uint32_t fill(uint32_t v) { return (1 << v) - 1; }; 
    template<typename T>
    constexpr T at(T const &val, uint32_t bit) { return (val >> bit) & 1; };
    template<typename T>
    constexpr void set(T &val, uint32_t bit) { val |= ((T) 1 << bit); };
    template<typename T>
    constexpr void unset(T &val, uint32_t bit) { val &= ~((T) 1 << bit); };

    constexpr uint8_t at_arr(uint8_t const *arr, uint32_t bit) { 
        return at(arr[bit >> 3], bit & 7);
    };
    constexpr void set_arr(uint8_t *arr, uint32_t bit) {
        set(arr[bit >> 3], bit & 7);
    };
    constexpr void unset_arr(uint8_t *arr, uint32_t bit) {
        unset(arr[bit >> 3], bit & 7);
    };
}


class PersistentFlag {
    uint8_t value : 1;
    uint8_t preserved : 1;
public:
    PersistentFlag() : value(0), preserved(0) {};
    void set(uint8_t v) { value = v; preserved |= v; }
    operator uint8_t() const { return preserved; };
    void clear() { preserved = value; };
};
