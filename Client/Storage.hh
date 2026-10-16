#pragma once

#include <cstdint>

namespace StorageProtocol {
const uint32_t MAX_LENGTH = 1024;
extern uint8_t buffer[2 * MAX_LENGTH];
void store(const char*, uint32_t);
uint32_t retrieve(const char*, uint32_t);

class Encoder {
public:
  uint8_t* base;
  uint8_t* at;
  Encoder(uint8_t*);

  template <typename T> void write(const T&);
};

class Decoder {
public:
  const uint8_t* base;
  const uint8_t* at;
  Decoder(const uint8_t*);

  template <typename T> T read();
};
} // namespace StorageProtocol

namespace Storage {
void retrieve();
void set();
} // namespace Storage