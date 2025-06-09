#include <Client/Storage.hh>

#include <cstring>

#include <emscripten.h>

using namespace Storage;

uint8_t Storage::buffer[2 * Storage::MAX_LENGTH] = {0};

static void _custom_b16_encode(uint32_t len) {
    for (uint32_t i = len; i > 0; --i) {
        uint8_t c = buffer[i - 1];
        buffer[2*i-1] = ((c >> 4) & 15) + 'a';
        buffer[2*i-2] = (c & 15) + 'a';
    }
}

static void _custom_b16_decode(uint32_t len) {
    for (uint32_t i = 0; i < len; ++i) {
        uint8_t lower = buffer[2*i] - 'a';
        uint8_t upper = buffer[2*i+1] - 'a';
        if (lower >= 16 || upper >= 16) break;
        buffer[i] = lower | (upper << 4);
    }
}

void Storage::store(char const *name, uint32_t len) {
    _custom_b16_encode(len);
    EM_ASM({
        window.localStorage[Module.TextDecoder.decode(HEAPU8.subarray($0,$0+$1))] = Module.TextDecoder.decode(HEAPU8.subarray($2,$2+$3));
    }, name, std::strlen(name), buffer, 2 * len);
}

uint32_t Storage::retrieve(char const *name, uint32_t max_length) {
    uint32_t len = EM_ASM_INT({
        let a = window.localStorage[Module.TextDecoder.decode(HEAPU8.subarray($0,$0+$1))];
        if (!a) return 0;
        a = a.slice(0, $3);
        HEAPU8.set(new TextEncoder().encode(a), $2);
        return a.length;
    }, name, std::strlen(name), buffer, 2 * max_length) / 2;
    _custom_b16_decode(len);
    return len;
}