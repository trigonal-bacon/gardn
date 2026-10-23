#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace StorageProtocol {
constexpr uint32_t MAX_LENGTH = 1024;
extern std::array<uint8_t, 2 * MAX_LENGTH> buffer;
void store(std::string_view, uint32_t);
uint32_t retrieve(std::string_view, uint32_t);

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