#pragma once

#include <Shared/EntityDef.hh>

#include <cstdint>
#include <string>
#include <vector>


enum Clientbound {
    kClientUpdate
};

enum Serverbound {
    kVerify,
    kClientInput,
    kClientSpawn,
    kPetalSwap,
    kPetalDelete,
    kChatSend
};

enum CloseReason {
    kServer = 4001,
    kProtocol = 4002,
    kOutdated = 4003,
    kRecovered = 4004
};

class Writer {
public:
    uint8_t *at;
    uint8_t *packet;
    template<typename T>
    class Encoder {
        friend class Writer;
        static void write(Writer &, T const &);
    };

    template<typename T>
    class Encoder<std::vector<T>> {
        friend class Writer;
        static void write(Writer &w, std::vector<T> const &v) {
            w.write<uint32_t>(v.size());
            for (T const &x : v) w.write<T>(x);
        };
    };

    Writer(uint8_t *);
    template<typename T>
    void write(T const &v) {
        Encoder<T>::write(*this, v);
    };
    void push(uint8_t);
};

class Reader {
public:
    template<typename T>
    class Decoder {
        friend class Reader;
        static T read(Reader &);
        static void read(Reader &, T &);
    };

    template<typename T>
    class Decoder<std::vector<T>> {
        friend class Reader;
        static std::vector<T> read(Reader &r) {
            uint32_t len = r.read<uint32_t>();
            std::vector<T> ret(len);
            for (uint32_t i = 0; i < len; ++i)
                r.read<T>(ret[i]);
            return ret;
        }

        static void read(Reader &r, std::vector<T> &v) {
            uint32_t len = r.read<uint32_t>();
            v.resize(len);
            for (uint32_t i = 0; i < len; ++i)
                r.read<T>(v[i]);
        }
    };

    uint8_t const *packet;
    uint8_t const *at;
    Reader(uint8_t const *);

    template<typename T>
    T read() {
        return Decoder<T>::read(*this);
    }

    template<typename T>
    void read(T &v) {
        Decoder<T>::read(*this, v);
    }

    uint8_t next();
};

class Validator {
public:
    uint8_t const *at;
    uint8_t const *end;
    Validator(uint8_t const *, uint8_t const *);

    uint8_t validate_uint8();
    uint8_t validate_uint32();
    uint8_t validate_uint64();
    uint8_t validate_float();
    uint8_t validate_string(uint32_t);
};
