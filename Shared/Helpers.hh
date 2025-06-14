#pragma once

#include <assert.h>

#include <chrono>
#include <cstdint>
#include <iostream>
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

#define TIME_FN(fn_body) { \
    struct timespec ts; \
    struct timespec te; \
    timespec_get(&ts, TIME_UTC); \
    fn_body; \
    timespec_get(&te, TIME_UTC); \
    double mss = ts.tv_sec * 1000 + ts.tv_nsec / 1000000.0; \
    double mse = te.tv_sec * 1000 + te.tv_nsec / 1000000.0; \
    std::printf("[%s]: %.4f\n", #fn_body, mse - mss); \
}

#define BIT_AT(val, bit) (((val) >> (bit)) & 1)
#define BIT_SET(val, bit) (val |= (1 << (bit)));
#define BIT_SHIFT(bit, shift) ((bit) << (shift))
#define BIT_AT_ARR(val, bit) (((val[(bit) >> 3]) >> ((bit) & 7)) & 1)
#define BIT_SET_ARR(val, bit) (val[(bit) >> 3]) |= (1 << ((bit) & 7));


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

template<typename T, uint32_t capacity>
class StaticArray {
public:
    T values[capacity];
    uint32_t length;
    StaticArray() : length(0) {};
    T &operator[](uint32_t at) { return values[at]; };
    void push(T val) { DEBUG_ONLY(assert(length < capacity); ) values[length++] = val; };
    void clear() { length = 0;}
    int32_t index_of(T val) const {
        for (uint32_t i = 0; i < length; ++i) if (values[i] == val) return i;
        return -1;
    };
};

template<typename T, uint32_t capacity>
class CircularArray {
    T values[capacity];
    uint32_t at;
    uint32_t length;
public:
    CircularArray() : at(0), length(0) {};
    uint32_t size() const { return length; }
    T operator[](uint32_t idx) const { return values[idx]; };
    void push(T val) { 
        values[at] = val; at = (at + 1) % capacity;
        if (length < capacity) ++length;
    };
    void clear() { 
        for (size_t i = 0; i < capacity; ++i) values[i] = {};
        length = 0;
        at = 0;
    };
};

class LerpFloat {
    float value;
    float lerp_value;
public:
    uint32_t touched;
    LerpFloat();
    void operator=(float);
    void set(float);
    operator float() const;
    void step(float);
    void step_angle(float);
};

SERVER_ONLY(typedef float Float;)
CLIENT_ONLY(typedef LerpFloat Float;)

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
    RangeValue(double, double);
    RangeValue(double);
    float get_single(float) const;
};
