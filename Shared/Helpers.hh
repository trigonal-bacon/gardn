#pragma once

#include <cassert>
#include <cstdint>
#include <format>
#include <string>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifdef CLIENTSIDE
#define CLIENT_ONLY(...) __VA_ARGS__
#else
#define CLIENT_ONLY(...)
#endif

#ifdef SERVERSIDE
#define SERVER_ONLY(...) __VA_ARGS__
#else
#define SERVER_ONLY(...)
#endif

#ifdef DEBUG
#define DEBUG_ONLY(...) __VA_ARGS__
#else
#define DEBUG_ONLY(...)
#endif

#define BIT_AT(val, bit) (((val) >> (bit)) & 1)
#define BIT_SET(val, bit) (val |= (1 << (bit)));
#define BIT_UNSET(val, bit) (val &= ~(1 << (bit)));
#define BIT_SHIFT(bit, shift) ((bit) << (shift))
#define BIT_AT_ARR(val, bit) (((val[(bit) >> 3]) >> ((bit) & 7)) & 1)
#define BIT_SET_ARR(val, bit) (val[(bit) >> 3]) |= (1 << ((bit) & 7));
#define BIT_UNSET_ARR(val, bit) (val[(bit) >> 3]) &= ~(1 << ((bit) & 7));

constexpr uint32_t bit_count(uint32_t v) {return 32 - __builtin_clz(v - 1); };
constexpr uint32_t bit_fill(uint32_t v) { return (1 << v) - 1; }; 
constexpr uint32_t div_round_up(uint32_t a, uint32_t b) { return (a + b - 1) / b; }

#define LERP(result, from, amt) { result = lerp(result, from, amt); }
#define ANGLE_LERP(result, from, amt) { result = angle_lerp(result, from, amt); }

double frand();
float fclamp(float, float, float);
float lerp(float, float, float);
float angle_lerp(float, float, float);

std::string format_pct(float);
std::string format_score(float);
std::string name_or_unnamed(std::string const &);

template<typename T, uint32_t capacity>
class StaticArray {
    T values[capacity];
    uint32_t length;
public:
    StaticArray() : length(0) {};
    constexpr StaticArray(std::initializer_list<T> elts) : length(0) {
        //static_assert(elts.size() <= capacity);
        for (T x : elts) values[length++] = x;
    };
    constexpr T &operator[](uint32_t at) { return values[at]; };
    constexpr T const &operator[](uint32_t at) const { return values[at]; };
    constexpr uint32_t size() const { return length; };
    constexpr void push(T val) { DEBUG_ONLY(assert(length < capacity); ) values[length++] = val; };
    constexpr T pop() { DEBUG_ONLY(assert(length > 0); ) return std::move(values[--length]); };
    constexpr void clear() { length = 0; }
    constexpr T *begin() { return &values[0]; };
    constexpr T *end() { return &values[length]; };
    constexpr T const *begin() const { return &values[0]; };
    constexpr T const *end() const { return &values[length]; };
};

template<typename T, uint32_t max_len>
class CircularArray {
    T values[max_len];
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

class LerpFloat {
    float value;
    float lerp_value;
    uint32_t touched;
public:
    LerpFloat();
    void operator=(float);
    void set(float);
    operator float() const;
    void step(float);
    void step_angle(float);
};

SERVER_ONLY(typedef float Float;)
CLIENT_ONLY(typedef LerpFloat Float;)

class PersistentFlag {
    uint8_t value : 1;
    uint8_t preserved : 1;
public:
    PersistentFlag();
    void set(uint8_t);
    operator uint8_t() const;
    void clear();
};

SERVER_ONLY(typedef uint8_t StickyFlag;)
CLIENT_ONLY(typedef PersistentFlag StickyFlag;)

class SeedGenerator {
    uint32_t seed;
public:
    SeedGenerator(uint32_t);
    float next();
    float binext();
};

class RangeValue {
public:
    float lower;
    float upper;
    RangeValue(float, float);
    RangeValue(float);
    float get_single(float) const;
    std::string const to_string() const;
};

class UTF8Parser {
    char const *str;
    char const *at;
    char next_char();
public:
    UTF8Parser(char const *);
    static std::string trunc_string(std::string const &, uint32_t);
    static bool is_valid_utf8(std::string const &);
    uint32_t next_symbol();
    uint32_t next_symbol_len() const;
    uint32_t offset() const;
};