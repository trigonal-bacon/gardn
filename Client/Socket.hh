#pragma once

#include <cstdint>
#include <string>

extern uint8_t INCOMING_PACKET[64 * 1024];
extern uint8_t OUTGOING_PACKET[1 * 1024];

class Socket {
public:
    uint8_t ready = 0;
    Socket();
    void connect(std::string const);
    void send(uint8_t *, uint32_t);
};