#include <Client/Storage.hh>

#include <Client/DOM.hh>
#include <Client/Game.hh>
#include <Client/Input.hh>

#include <cstring>
#include <emscripten.h>


uint8_t StorageProtocol::buffer[2 * StorageProtocol::MAX_LENGTH] = {0};

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

void StorageProtocol::store(char const *name, uint32_t len) {
    _custom_b16_encode(len);
    EM_ASM({
        window.localStorage[Module.TextDecoder.decode(HEAPU8.subarray($0,$0+$1))] = Module.TextDecoder.decode(HEAPU8.subarray($2,$2+$3));
    }, name, std::strlen(name), buffer, 2 * len);
}

uint32_t StorageProtocol::retrieve(char const *name, uint32_t max_length) {
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

void Storage::retrieve() {
    Game::seen_petals[PetalID::kBasic] = 1;
    {
        uint32_t len = StorageProtocol::retrieve("mobs", 256);
        Reader reader(&StorageProtocol::buffer[0]);
        while (reader.at < StorageProtocol::buffer + len) {
            MobID::T mob_id = reader.read<uint8_t>();
            if (mob_id >= MobID::kNumMobs) break;
            Game::seen_mobs[mob_id] = 1;
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("petals", 256);
        Reader reader(&StorageProtocol::buffer[0]);
        while (reader.at < StorageProtocol::buffer + len) {
            PetalID::T petal_id = reader.read<uint8_t>();
            if (petal_id >= PetalID::kNumPetals || petal_id == PetalID::kNone) break;
            Game::seen_petals[petal_id] = 1;
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("settings", 1);
        if (len > 0) {
            Reader reader(&StorageProtocol::buffer[0]);
            uint8_t opts = reader.read<uint8_t>();
            Input::movement_helper = BIT_AT(opts, 0);
            Input::keyboard_movement = BIT_AT(opts, 1);
        }
    }
    {
        uint32_t len = StorageProtocol::retrieve("nickname", 72);
        if (len > 0) {
            Reader reader(&StorageProtocol::buffer[0]);
            reader.read<std::string>(Game::nickname);
            DOM::update_text("t0", Game::nickname, 16);
        }
    }
}

void Storage::set() {
    #define X(ct, name) checker_##ct.cmp(name);
    STORED
    #undef X
    if (should_update == 0) return;
    should_update = 0;
    {
        Writer writer(&StorageProtocol::buffer[0]);
        for (PetalID::T id = PetalID::kBasic; id < PetalID::kNumPetals; ++id)
            if (Game::seen_petals[id]) writer.write<uint8_t>(id);
        StorageProtocol::store("petals", writer.at - writer.packet);
    }
    {
        Writer writer(&StorageProtocol::buffer[0]);
        for (MobID::T id = 0; id < MobID::kNumMobs; ++id)
            if (Game::seen_mobs[id]) writer.write<uint8_t>(id);
        StorageProtocol::store("mobs", writer.at - writer.packet);
    }
    {
        Writer writer(&StorageProtocol::buffer[0]);
        writer.write<std::string>(Game::nickname);
        StorageProtocol::store("nickname", writer.at - writer.packet);
    }
    {
        Writer writer(&StorageProtocol::buffer[0]);
        writer.write<uint8_t>(
            Input::movement_helper | (Input::keyboard_movement << 1)
        );
        StorageProtocol::store("settings", writer.at - writer.packet);
    }
}