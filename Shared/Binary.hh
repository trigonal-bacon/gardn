#pragma once

#include <Shared/EntityDef.hh>
#include <Shared/Helpers.hh>

#include <cstdint>
#include <string>

enum Clientbound {
    kDisconnect,
    kClientUpdate,
    kOutdated
};

enum Serverbound {
    kVerify,
    kClientInput,
    kClientSpawn,
    kPetalSwap,
    kPetalDelete
};

class Writer {
public:
    uint8_t *packet;
    uint8_t *at;
    Writer(uint8_t *);
    
    template<typename T>
    void write(T const &);
};

class Reader {
public:
    uint8_t const *packet;
    uint8_t const *at;
    Reader(uint8_t const *);

    template<typename T>
    T read();

    template<typename T>
    void read(T &);
};

class Validator {
public:
    uint8_t const *at;
    uint8_t const *end;
    Validator(uint8_t const *, uint8_t const *);

    uint8_t validate_uint8();
    uint8_t validate_uint32();
    uint8_t validate_uint64();
    uint8_t validate_float();
    uint8_t validate_string(uint32_t);
};
