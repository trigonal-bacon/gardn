#pragma once

#include <cstdint>
#include <string>

class UTF8Parser {
    char const *str;
    char const *at;
    char const *end;
    char next_char();
public:
    UTF8Parser(char const *, size_t);
    UTF8Parser(std::string const &);
    static std::string trunc_string(std::string const &, uint32_t);
    static bool is_valid_utf8(std::string const &);
    uint32_t next_symbol();
    uint32_t next_symbol_len() const;
    uint32_t offset() const;
    bool at_end() const;
};