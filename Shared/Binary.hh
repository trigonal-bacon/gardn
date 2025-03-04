#pragma once

#include <Shared/EntityDef.hh>
#include <Shared/Helpers.hh>

#include <stdint.h>

#include <string>

enum kClientbound {
    kClientUpdate
};

enum kServerbound {
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
    void write_uint8(uint8_t);
    void write_uint32(uint32_t);
    void write_int32(int32_t);
    void write_float(float);
    void write_entid(EntityId const &);
    void write_string(std::string const &);
};

class Reader {
public:
    uint8_t const *packet;
    uint8_t const *at;
    Reader(uint8_t const *);
    uint8_t read_uint8();
    uint32_t read_uint32();
    int32_t read_int32();
    float read_float();
    EntityId read_entid();

    void read_uint8(uint8_t &);
    void read_uint32(uint32_t &);
    void read_int32(int32_t &);
    void read_float(float &);
    void read_entid(EntityId &);
    void read_string(std::string &);
};