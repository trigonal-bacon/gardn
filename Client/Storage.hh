#pragma once

#include <cstdint>

namespace StorageProtocol {
    uint32_t const MAX_LENGTH = 1024;
    extern uint8_t buffer[2 * MAX_LENGTH];
    void store(char const *, uint32_t);
    uint32_t retrieve(char const *, uint32_t);

    class Encoder {
    public:
        uint8_t *base;
        uint8_t *at;
        Encoder(uint8_t *);
        
        template<typename T>
        void write(T const &);
    };

    class Decoder {
    public:
        uint8_t const *base;
        uint8_t const *at;
        Decoder(uint8_t const *);
        
        template<typename T>
        T read();
    };
}

namespace Storage {
    void retrieve();
    void set();
}