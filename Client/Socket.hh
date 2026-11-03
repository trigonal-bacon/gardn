#pragma once

#include <Shared/Config.hh>

#include <cstdint>
#include <string>

extern uint8_t INCOMING_PACKET[MAX_CLIENTBOUND_PACKET_LEN];
extern uint8_t OUTGOING_PACKET[MAX_SERVERBOUND_PACKET_LEN];

class Socket {
public:
  uint8_t ready = 0;
  Socket();
  void connect(std::string_view);
  void send(uint8_t*, uint32_t);
};