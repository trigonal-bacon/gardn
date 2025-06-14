#pragma once

#include <cstdint>
#include <string>

extern uint8_t INCOMING_PACKET[1024 * 1024];
class Socket {
public:
    uint8_t ready = 0;
    Socket();
    void connect(std::string const);
    void send(uint8_t *, uint32_t);
};