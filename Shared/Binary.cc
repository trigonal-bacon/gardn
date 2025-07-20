#include <Shared/Binary.hh>

#include <iostream>

static const uint32_t PROTOCOL_FLOAT_SCALE = 64;

Writer::Writer(uint8_t *buf) : at(buf), packet(buf) {}

template<>
void Writer::write<uint8_t>(uint8_t const &val) {
    *at = val;
    ++at;
}

template<>
void Writer::write<uint16_t>(uint16_t const &val) {
    uint16_t v = val;
    while (v > 127)
    {
        write<uint8_t>((v & 127) | 128);
        v >>= 7;
    }
    write<uint8_t>(v);
}

template<>
void Writer::write<uint32_t>(uint32_t const &val) {
    uint32_t v = val;
    while (v > 127)
    {
        write<uint8_t>((v & 127) | 128);
        v >>= 7;
    }
    write<uint8_t>(v);
}

template<>
void Writer::write<uint64_t>(uint64_t const &val) {
    uint64_t v = val;
    while (v > 127ll)
    {
        write<uint8_t>((v & 127ll) | 128ll);
        v >>= 7ll;
    }
    write<uint8_t>(v);
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


Reader::Reader(uint8_t const *buf) : at(buf), packet(buf) {}

template<>
uint8_t Reader::read<uint8_t>() {
    return *at++;
}


template<>
uint16_t Reader::read<uint16_t>() {
    uint16_t ret = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        uint8_t o = read<uint8_t>();
        ret |= ((o & 127) << (i * 7));
        if (o <= 127) break;
    }
    return ret;
}

template<>
uint32_t Reader::read<uint32_t>() {
    uint32_t ret = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        uint8_t o = read<uint8_t>();
        ret |= ((o & 127) << (i * 7));
        if (o <= 127) break;
    }
    return ret;
}

template<>
uint64_t Reader::read<uint64_t>() {
    uint64_t ret = 0ll;
    for (uint32_t i = 0; i < 10; ++i) {
        uint8_t o = read<uint8_t>();
        ret |= ((o & 127ll) << (i * 7ll));
        if (o <= 127ll) break;
    }
    return ret;
}

template<>
int32_t Reader::read<int32_t>() {
    uint32_t r = read<uint32_t>();
    uint32_t s = r & 1;
    int32_t ret = r >> 1;
    if (s) ret *= -1;
    return ret;
}

template<>
int64_t Reader::read<int64_t>() {
    uint64_t r = read<uint64_t>();
    uint32_t s = r & 1;
    int64_t ret = r >> 1;
    if (s) ret *= -1;
    return ret;
}

template<>
float Reader::read<float>() {
    return read<int64_t>() / (float) PROTOCOL_FLOAT_SCALE;
}

template<>
double Reader::read<double>() {
    return read<int64_t>() / (double) PROTOCOL_FLOAT_SCALE;
}

template<>
EntityID Reader::read<EntityID>() {
    EntityID::id_type id = read<EntityID::id_type>();
    EntityID::hash_type hash = id ? read<EntityID::hash_type>() : 0;
    return {id, hash};
}

template<>
void Reader::read<uint8_t>(uint8_t &ref) {
    ref = read<uint8_t>();
}

template<>
void Reader::read<uint16_t>(uint16_t &ref) {
    ref = read<uint16_t>();
}

template<>
void Reader::read<uint32_t>(uint32_t &ref) {
    ref = read<uint32_t>();
}

template<>
void Reader::read<uint64_t>(uint64_t &ref) {
    ref = read<uint64_t>();
}

template<>
void Reader::read<int32_t>(int32_t &ref) {
    ref = read<int32_t>();
}

template<>
void Reader::read<int64_t>(int64_t &ref) {
    ref = read<int64_t>();
}


template<>
void Reader::read<float>(float &ref) {
    ref = read<float>();
}

template<>
void Reader::read<double>(double &ref) {
    ref = read<double>();
}

template<>
void Reader::read<LerpFloat>(LerpFloat &ref) {
    ref.set(read<float>());
}

template<>
void Reader::read<EntityID>(EntityID &ref) {
    ref = read<EntityID>();
}

template<>
void Reader::read<std::string>(std::string &ref) {
    uint32_t len = read<uint32_t>();
    ref.clear();
    ref.reserve(len);
    for (uint32_t i = 0; i < len; ++i) ref.push_back(read<uint8_t>());
}

Validator::Validator(uint8_t const *start, uint8_t const *end) : at(start), end(end) {}

uint8_t Validator::validate_uint8() {
    return (at += sizeof(uint8_t)) <= end;
}

uint8_t Validator::validate_uint32() {
    for (uint8_t i = 0; i < 5; ++i) {
        uint8_t x = *at;
        if (!validate_uint8()) return 0;
        if (x <= 127) return 1;
    }
    return 0;
}

uint8_t Validator::validate_uint64() {
    for (uint8_t i = 0; i < 10; ++i) {
        uint8_t x = *at;
        if (!validate_uint8()) return 0;
        if (x <= 127) return 1;
    }
    return 0;
}

uint8_t Validator::validate_float() {
    return validate_uint32();
}

uint8_t Validator::validate_string(uint32_t max_len) {
    uint8_t const *old = at;
    if (!validate_uint32()) return 0;
    at = old;
    uint32_t len = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        uint8_t o = *at++;
        len |= ((o & 127) << (i * 7));
        if (o <= 127) break;
    }
    if (len > max_len) return 0;
    return (at += len) <= end;
}
