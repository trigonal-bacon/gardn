#pragma once

#include <cstdint>

namespace Storage {
    uint32_t const MAX_LENGTH = 1024;
    extern uint8_t buffer[2 * MAX_LENGTH];
    void store(char const *, uint32_t);
    uint32_t retrieve(char const *, uint32_t);
}