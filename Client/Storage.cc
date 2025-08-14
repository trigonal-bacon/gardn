#include <Client/Storage.hh>

#include <Client/Game.hh>
#include <Client/Input.hh>

#include <Shared/Config.hh>

#include <cstring>
#include <string>
#include <emscripten.h>

uint32_t const XOR_CYCLE_LENGTH = 7;
static uint8_t const XOR_CYCLE[XOR_CYCLE_LENGTH] = { 0x43, 0x37, 0x92, 0xa4, 0x38, 0xf1, 0x2b };

static void _custom_b16_encode(uint32_t len) {
    for (uint32_t i = len; i > 0; --i) {
        uint8_t c = StorageProtocol::buffer[i - 1];
        StorageProtocol::buffer[2*i-1] = ((c >> 4) & 15) + 'a';
        StorageProtocol::buffer[2*i-2] = (c & 15) + 'a';
    }
}

static void _custom_b16_decode(uint32_t len) {
    for (uint32_t i = 0; i < len; ++i) {
        uint8_t lower = StorageProtocol::buffer[2*i] - 'a';
        uint8_t upper = StorageProtocol::buffer[2*i+1] - 'a';
        if (lower >= 16 || upper >= 16) break;
        StorageProtocol::buffer[i] = lower | (upper << 4);
    }
}

namespace StorageProtocol {
    uint8_t buffer[2 * StorageProtocol::MAX_LENGTH] = {0};

    void store(char const *name, uint32_t len) {
        _custom_b16_encode(len);
        EM_ASM({
            window.localStorage[Module.TextDecoder.decode(HEAPU8.subarray($0,$0+$1))] = Module.TextDecoder.decode(HEAPU8.subarray($2,$2+$3));
        }, name, std::strlen(name), buffer, 2 * len);
    }

    uint32_t retrieve(char const *name, uint32_t max_length) {
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

    Encoder::Encoder(uint8_t *ptr) : base(ptr), at(ptr) {}

    template<>
    void Encoder::write<uint8_t>(uint8_t const &o) {
        *at = o ^ (XOR_CYCLE[(at - base) % XOR_CYCLE_LENGTH]);
        ++at;
    }

    template<>
    void Encoder::write<uint32_t>(uint32_t const &val) {
        uint32_t v = val;
        while (v > 127)
        {
            write<uint8_t>((v & 127) | 128);
            v >>= 7;
        }
        write<uint8_t>(v);
    }

    template<>
    void Encoder::write<std::string>(std::string const &str) {
        uint32_t len = str.size();
        write<uint32_t>(len);
        for (uint32_t i = 0; i < len; ++i) write<uint8_t>(str[i]);
    }

    Decoder::Decoder(uint8_t const *ptr) : base(ptr), at(ptr) {}

    template<>
    uint8_t Decoder::read<uint8_t>() {
        uint8_t ret = *at ^ (XOR_CYCLE[(at - base) % XOR_CYCLE_LENGTH]);
        ++at;
        return ret;
    }

    template<>
    uint32_t Decoder::read<uint32_t>() {
        uint32_t ret = 0;
        for (uint32_t i = 0; i < 5; ++i) {
            uint8_t o = read<uint8_t>();
            ret |= ((o & 127) << (i * 7));
            if (o <= 127) break;
        }
        return ret;
    }

    template<>
    std::string Decoder::read<std::string>() {
        std::string ref;
        uint32_t len = read<uint32_t>();
        for (uint32_t i = 0; i < len; ++i) ref.push_back(read<uint8_t>());
        return ref;
    }
}

static bool should_update = 0;

template<typename T>
class MutationObserver {
    T prev = {};
public:
    MutationObserver(T const &o) : prev(o) {};
    void cmp(T &o) {
        if (o == prev) return;
        should_update = 1;
        prev = o;
        return;
    }
};

#define STORED \
    X(0, Game::nickname) \
    X(1, Game::seen_mobs) \
    X(2, Game::seen_petals) \
    X(3, Input::keyboard_movement) \
    X(4, Input::movement_helper)

#define X(ct, name) static auto checker_##ct = MutationObserver(name);
STORED
#undef X

using namespace StorageProtocol;

void Storage::retrieve() {
    Game::seen_petals[PetalID::kBasic] = 1;
    {
        uint32_t len = StorageProtocol::retrieve("mobs", 256);
        Decoder reader(&StorageProtocol::buffer[0]);
        while (reader.at < StorageProtocol::buffer + len) {
            MobID::T mob_id = reader.read<uint8_t>();
            if (mob_id >= MobID::kNumMobs) break;
            Game::seen_mobs[mob_id] = 1;
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("petals", 256);
        Decoder reader(&StorageProtocol::buffer[0]);
        while (reader.at < StorageProtocol::buffer + len) {
            PetalID::T petal_id = reader.read<uint8_t>();
            if (petal_id >= PetalID::kNumPetals || petal_id == PetalID::kNone) break;
            Game::seen_petals[petal_id] = 1;
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("settings", 1);
        if (len > 0) {
            Decoder reader(&StorageProtocol::buffer[0]);
            uint8_t opts = reader.read<uint8_t>();
            Input::movement_helper = BIT_AT(opts, 0);
            Input::keyboard_movement = BIT_AT(opts, 1);
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("nickname", MAX_NAME_LENGTH + 4);
        if (len > 0 && len <= MAX_NAME_LENGTH + 4) {
            Decoder reader(&StorageProtocol::buffer[0]);
            Game::nickname = reader.read<std::string>();
        }
    }
    #ifdef DEBUG
    for (MobID::T i = 0; i < MobID::kNumMobs; ++i) Game::seen_mobs[i] = 1;
    for (PetalID::T i = PetalID::kBasic; i < PetalID::kNumPetals; ++i) Game::seen_petals[i] = 1;
    #endif
}

void Storage::set() {
    #define X(ct, name) checker_##ct.cmp(name);
    STORED
    #undef X
    if (should_update == 0) return;
    should_update = 0;
    {
        Encoder writer(&StorageProtocol::buffer[0]);
        for (PetalID::T id = PetalID::kBasic; id < PetalID::kNumPetals; ++id)
            if (Game::seen_petals[id]) writer.write<uint8_t>(id);
        StorageProtocol::store("petals", writer.at - writer.base);
    }
    {
        Encoder writer(&StorageProtocol::buffer[0]);
        for (MobID::T id = 0; id < MobID::kNumMobs; ++id)
            if (Game::seen_mobs[id]) writer.write<uint8_t>(id);
        StorageProtocol::store("mobs", writer.at - writer.base);
    }
    {
        Encoder writer(&StorageProtocol::buffer[0]);
        writer.write<std::string>(Game::nickname);
        StorageProtocol::store("nickname", writer.at - writer.base);
    }
    {
        Encoder writer(&StorageProtocol::buffer[0]);
        writer.write<uint8_t>(
            Input::movement_helper | (Input::keyboard_movement << 1)
        );
        StorageProtocol::store("settings", writer.at - writer.base);
    }
}