#pragma once

#include <cstdint>
#include <string>

constexpr size_t MAX_CLIENTBOUND_PACKET_LEN = 1024 * 1024;
constexpr size_t MAX_SERVERBOUND_PACKET_LEN = 1 * 1024;

extern const std::string WS_URL;
extern const uint64_t VERSION_HASH;
extern const uint32_t SERVER_PORT;
extern const uint32_t MAX_NAME_LENGTH;