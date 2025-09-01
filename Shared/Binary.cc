#include <Shared/Binary.hh>

#include <Helpers/Bits.hh>
#include <Helpers/UTF8.hh>

static const uint32_t PROTOCOL_FLOAT_SCALE = 64;


Writer::Writer(uint8_t *v) : at(v), packet(v) {}

void Writer::push(uint8_t val) {
    *at++ = val;
}

template<>
void Writer::Encoder<uint8_t>::write(Writer &w, uint8_t const &val) {
    w.push(val);
}

template<>
void Writer::Encoder<uint16_t>::write(Writer &w, uint16_t const &val) {
    uint16_t v = val;
    while (v > 127) {
        w.write<uint8_t>((v & 127) | 128);
        v >>= 7;
    }
    w.write<uint8_t>(v);
}

template<>
void Writer::Encoder<uint32_t>::write(Writer &w, uint32_t const &val) {
    uint32_t v = val;
    while (v > 127) {
        w.write<uint8_t>((v & 127) | 128);
        v >>= 7;
    }
    w.write<uint8_t>(v);
}

template<>
void Writer::Encoder<uint64_t>::write(Writer &w, uint64_t const &val) {
    uint64_t v = val;
    while (v > 127) {
        w.write<uint8_t>((v & 127) | 128);
        v >>= 7;
    }
    w.write<uint8_t>(v);
}

template<>
void Writer::Encoder<int32_t>::write(Writer &w, int32_t const &val) {
    int32_t v = val;
    uint32_t sign = v < 0;
    if (sign) v *= -1;
    v = (v << 1) | sign;
    w.write<uint64_t>(v);
}

template<>
void Writer::Encoder<int64_t>::write(Writer &w, int64_t const &val) {
    int64_t v = val;
    uint32_t sign = v < 0;
    if (sign) v *= -1;
    v = (v << 1) | sign;
    w.write<uint64_t>(v);
}

template<>
void Writer::Encoder<float>::write(Writer &w, float const &v) {
    w.write<int64_t>(v * PROTOCOL_FLOAT_SCALE);
}

template<>
void Writer::Encoder<double>::write(Writer &w, double const &v) {
    w.write<int64_t>(v * PROTOCOL_FLOAT_SCALE);
}

template<>
void Writer::Encoder<EntityID>::write(Writer &w, EntityID const &id) {
    w.write<EntityID::id_type>(id.id);
    if (id.id) w.write<EntityID::hash_type>(id.hash);
}

template<>
void Writer::Encoder<std::string>::write(Writer &w, std::string const &str) {
    uint32_t len = str.size();
    w.write<uint32_t>(len);
    for (uint32_t i = 0; i < len; ++i) w.write<uint8_t>(str[i]);
}

Reader::Reader(uint8_t const *buf) : at(buf), packet(buf) {}

uint8_t Reader::next() {
    return *at++;
}

template<>
uint8_t Reader::Decoder<uint8_t>::read(Reader &r) {
    return r.next();
}

template<>
uint16_t Reader::Decoder<uint16_t>::read(Reader &r) {
    uint16_t ret = 0;
    for (uint32_t i = 0; i < 3; ++i) {
        uint8_t o = r.read<uint8_t>();
        ret |= ((o & 127) << (i * 7));
        if (o <= 127) break;
    }
    return ret;
}

template<>
uint32_t Reader::Decoder<uint32_t>::read(Reader &r) {
    uint32_t ret = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        uint8_t o = r.read<uint8_t>();
        ret |= ((o & 127) << (i * 7));
        if (o <= 127) break;
    }
    return ret;
}

template<>
uint64_t Reader::Decoder<uint64_t>::read(Reader &r) {
    uint64_t ret = 0ll;
    for (uint32_t i = 0; i < 10; ++i) {
        uint8_t o = r.read<uint8_t>();
        ret |= ((o & 127ll) << (i * 7ll));
        if (o <= 127ll) break;
    }
    return ret;
}

template<>
int32_t Reader::Decoder<int32_t>::read(Reader &r) {
    uint32_t u = r.read<uint32_t>();
    uint32_t s = u & 1;
    int32_t ret = u >> 1;
    if (s) ret *= -1;
    return ret;
}

template<>
int64_t Reader::Decoder<int64_t>::read(Reader &r) {
    uint64_t u = r.read<uint64_t>();
    uint32_t s = u & 1;
    int64_t ret = u >> 1;
    if (s) ret *= -1;
    return ret;
}

template<>
float Reader::Decoder<float>::read(Reader &r) {
    return r.read<int64_t>() / (float) PROTOCOL_FLOAT_SCALE;
}

template<>
double Reader::Decoder<double>::read(Reader &r) {
    return r.read<int64_t>() / (double) PROTOCOL_FLOAT_SCALE;
}

template<>
EntityID Reader::Decoder<EntityID>::read(Reader &r) {
    EntityID::id_type id = r.read<EntityID::id_type>();
    EntityID::hash_type hash = id ? r.read<EntityID::hash_type>() : 0;
    return EntityID(id, hash);
}

template<>
void Reader::Decoder<uint8_t>::read(Reader &r, uint8_t &ref) {
    ref = r.read<uint8_t>();
}

template<>
void Reader::Decoder<uint16_t>::read(Reader &r, uint16_t &ref) {
    ref = r.read<uint16_t>();
}

template<>
void Reader::Decoder<uint32_t>::read(Reader &r, uint32_t &ref) {
    ref = r.read<uint32_t>();
}

template<>
void Reader::Decoder<uint64_t>::read(Reader &r, uint64_t &ref) {
    ref = r.read<uint64_t>();
}

template<>
void Reader::Decoder<int32_t>::read(Reader &r, int32_t &ref) {
    ref = r.read<int32_t>();
}

template<>
void Reader::Decoder<int64_t>::read(Reader &r, int64_t &ref) {
    ref = r.read<int64_t>();
}

template<>
void Reader::Decoder<float>::read(Reader &r, float &ref) {
    ref = r.read<float>();
}

template<>
void Reader::Decoder<double>::read(Reader &r, double &ref) {
    ref = r.read<double>();
}

template<>
void Reader::Decoder<LerpFloat>::read(Reader &r, LerpFloat &ref) {
    ref.set(r.read<float>());
}

template<>
void Reader::Decoder<EntityID>::read(Reader &r, EntityID &ref) {
    ref = r.read<EntityID>();
}

template<>
void Reader::Decoder<std::string>::read(Reader &r, std::string &ref) {
    uint32_t len = r.read<uint32_t>();
    ref.clear();
    ref.reserve(len);
    for (uint32_t i = 0; i < len; ++i) ref.push_back(r.read<uint8_t>());
}

template<>
void Reader::Decoder<PersistentFlag>::read(Reader &r, PersistentFlag &ref) {
    ref.set(r.read<uint8_t>());
}

Validator::Validator(uint8_t const *start, uint8_t const *end) : at(start), end(end) {}

uint8_t Validator::validate_uint8() {
    return (at += sizeof(uint8_t)) <= end;
}

uint8_t Validator::validate_uint32() {
    if (at >= end) return 0;
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
    Reader reader(old);
    uint32_t byte_len = reader.read<uint32_t>();
#ifdef USE_CODEPOINT_LEN
    if (byte_len == 0) return 1;
    if (byte_len + at > end) return 0;
    old = at + byte_len;
    UTF8Parser utf8_parser(reinterpret_cast<char const *>(at));
    for (uint32_t i = 0; i < max_len; ++i) {
        at += utf8_parser.next_symbol_len();
        if (at < old) {
            utf8_parser.next_symbol();
            continue;
        }
        return (at == old);
    }
    return 0;
#else
    if (byte_len > max_len) return 0;
    return (at += byte_len) <= end;
#endif
}
