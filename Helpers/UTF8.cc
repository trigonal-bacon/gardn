#include <Helpers/UTF8.hh>

UTF8Parser::UTF8Parser(char const *s, size_t len) : str(s), at(s), end(s + len) {}

UTF8Parser::UTF8Parser(std::string const &str) : UTF8Parser(str.c_str(), str.size()) {}

std::string UTF8Parser::trunc_string(std::string const &str, uint32_t max) {
    UTF8Parser parser(str);
#ifdef USE_CODEPOINT_LEN
    for (uint32_t i = 0; i < max; ++i) {
        if (parser.at_end()) break;
        if (parser.next_symbol() == 0) break;
    }
    return str.substr(0, parser.offset());
#else
    uint32_t last = 0;
    while (last <= max) {
        if (parser.next_symbol() == 0) break;
        uint32_t len = parser.offset();
        if (len > max) break;
        last = len;
    }
    return str.substr(0, last);
#endif
}

char UTF8Parser::next_char() {
    if (*at == 0) return 0;
    if (at >= end) return 0;
    return *at++;
}

bool UTF8Parser::is_valid_utf8(std::string const &str) {
    uint32_t at = 0;
    while (at < str.size()) {
        uint8_t c = (uint8_t) str[at++];
        if (c < 0x80) continue;
        uint32_t run_len;
        if (c >= 0xf0)
            run_len = 3;
        else if (c >= 0xe0)
            run_len = 2;
        else
            run_len = 1;
        if (at + run_len > str.size()) return false;
        for (uint32_t i = 0; i < run_len; ++i, ++at)
            if (((uint8_t) (str[at] ^ 0x80)) >= 0x40) return false;
    }
    return true;
}

uint32_t UTF8Parser::next_symbol() {
    uint32_t c = (uint8_t) next_char();
    if (c & 0x80) {
        if (c >= 0xf0) {
            c &= 0x07;
            for (uint32_t x = 0; x < 3; ++x)
                c = (c << 6) | (next_char() & 0x3f);
        } else if (c >= 0xe0) {
            c &= 0x0f;
            for (uint32_t x = 0; x < 2; ++x)
                c = (c << 6) | (next_char() & 0x3f);
        } else {
            c &= 0x1f;
            for (uint32_t x = 0; x < 1; ++x)
                c = (c << 6) | (next_char() & 0x3f);
        }
    }
    return c;
}

uint32_t UTF8Parser::next_symbol_len() const {
    uint8_t c = (uint8_t) *at;
    if (c < 0x80) return 1;
    if (c < 0xe0) return 2;
    if (c < 0xf0) return 3;
    return 4;
}

uint32_t UTF8Parser::offset() const {
    return at - str;
}

bool UTF8Parser::at_end() const {
    return at >= end || next_symbol_len() + at > end;
}