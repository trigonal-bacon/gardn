#pragma once

#include <cstdint>
#include <string>

class UTF8Parser {
  const char* str;
  const char* at;
  const char* end;
  char next_char();
  uint32_t fail();

public:
  UTF8Parser(const char*, size_t);
  UTF8Parser(std::string_view);
  static std::string trunc_string(std::string_view, uint32_t);
  static bool is_valid_utf8(std::string_view);
  uint32_t next_symbol();
  uint32_t next_symbol_len() const;
  uint32_t offset() const;
  bool at_end() const;
};