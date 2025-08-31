#include <Shared/Binary_.hh>

#include <Shared/Entity.hh>

static const uint32_t PROTOCOL_FLOAT_SCALE = 64;

Writer::Writer(uint8_t *v) : at(v), packet(v) {}

void Writer::push(uint8_t val) {
    *packet++ = val;
}

template<>
void Writer::Encoder<uint8_t>::write(Writer &w, uint8_t const &val) {
    w.push(val);
}

template<>
void Writer::Encoder<uint16_t>::write(Writer &w, uint16_t const &val) {
    uint16_t v = val;
    while (v > 127) {
        Writer::Encoder<uint8_t>::write(w, (v & 127) | 128);
        v >>= 7;
    }
}

template<>
void Writer::Encoder<uint32_t>::write(Writer &w, uint32_t const &val) {
    uint32_t v = val;
    while (v > 127) {
        Writer::Encoder<uint8_t>::write(w, (v & 127) | 128);
        v >>= 7;
    }
}

template<>
void Writer::Encoder<uint64_t>::write(Writer &w, uint64_t const &val) {
    uint64_t v = val;
    while (v > 127) {
        Writer::Encoder<uint8_t>::write(w, (v & 127) | 128);
        v >>= 7;
    }
}

template<>
void Writer::write<int32_t>(int32_t const &val) {
    int32_t v = val;
    uint32_t sign = v < 0;
    if (sign) v *= -1;
    v = (v << 1) | sign;
    write<uint32_t>(v);
}

template<>
void Writer::write<int64_t>(int64_t const &val) {
    int64_t v = val;
    uint32_t sign = v < 0;
    if (sign) v *= -1;
    v = (v << 1) | sign;
    write<uint64_t>(v);
}

template<>
void Writer::write<float>(float const &v) {
    write<int64_t>(v * PROTOCOL_FLOAT_SCALE);
}

template<>
void Writer::write<double>(double const &v) {
    write<int64_t>(v * PROTOCOL_FLOAT_SCALE);
}

template<>
void Writer::write<EntityID>(EntityID const &id) {
    write<EntityID::id_type>(id.id);
    if (id.id) write<EntityID::hash_type>(id.hash);
}

template<>
void Writer::write<std::string>(std::string const &str) {
    uint32_t len = str.size();
    write<uint32_t>(len);
    for (uint32_t i = 0; i < len; ++i) write<uint8_t>(str[i]);
}