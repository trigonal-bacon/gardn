#include <Shared/Helpers.hh>

#include <random>
#include <string>

float fclamp(float v, float s, float e) {
    if (!(v >= s)) return s;
    if (!(v <= e)) return e;
    return v;
}

double frand() {
    return std::rand() / (double) RAND_MAX;
}


float lerp(float v, float e, float a) {
    a = fclamp(a, 0, 1);
    return v * (1 - a) + e * a;
}

float angle_lerp(float start, float end, float t)
{
    t = fclamp(t, 0, 1);

    start = fmod(start, 2 * M_PI);
    end = fmod(end, 2 * M_PI);
    if (fabs(end - start) < M_PI)
        return (end - start) * t + start;
    else
    {
        if (end > start)
            start += 2 * M_PI;
        else
            end += 2 * M_PI;
        return fmod((end - start) * t + start + 2 * M_PI, 2 * M_PI);
    }
}

std::string format_pct(float pct) {
    if (pct >= 0.9) return std::format("{:.0f}%", pct);
    if (pct >= 0.09) return std::format(".{:.0f}%", pct * 10);
    if (pct >= 0.009) return std::format(".0{:.0f}%", pct * 100);
    return std::format(".00{:.0}%", pct * 1000);
}

std::string format_score(float score) {
    if (score <= 1000) return std::format("{:.0f}", score);
    if (score < 1000000) return std::format("{:.1f}k", score / 1000);
    return std::format("{:.1f}m", score / 1000000);
}

std::string name_or_unnamed(std::string const &name) {
    if (name.size() == 0) return "Unnamed";
    return name;
}

LerpFloat::LerpFloat() {
    value = lerp_value = 0;
    touched = 0;
}

void LerpFloat::operator=(float v) {
    value = lerp_value = v;
    touched = 0;
}

void LerpFloat::set(float v) {
    value = v;
    if (!touched) {
        touched = 1;
        lerp_value = v;
    }
}

LerpFloat::operator float() const {
    return lerp_value;
}

void LerpFloat::step(float amt) {
    lerp_value = lerp(lerp_value, value, amt);
}

void LerpFloat::step_angle(float amt) {
    lerp_value = angle_lerp(lerp_value, value, amt);
}

PersistentFlag::PersistentFlag() : value(0), preserved(0) {}

void PersistentFlag::set(uint8_t v) {
    value = v;
    preserved |= v;
}

PersistentFlag::operator uint8_t() const {
    return preserved;
}

void PersistentFlag::clear() {
    preserved = value;
}

SeedGenerator::SeedGenerator(uint32_t s) : seed(s) {}

float SeedGenerator::next() {
    seed *= 167436543;
    seed += 5832385;
    seed *= (76372345 + seed);
    seed += 937323;
    return (seed % 65536) / 65536.0f;
}

float SeedGenerator::binext() {
    return next() * 2 - 1;
}

RangeValue::RangeValue(float l, float u) : lower(l), upper(u) {}

RangeValue::RangeValue(float l) : lower(l), upper(l) {}

float RangeValue::get_single(float a) const {
    if (lower == upper) return lower;
    return lower + (upper - lower) * fclamp(a, 0, 1);
}

std::string const RangeValue::to_string() const {
    if (lower == upper) return format_score(lower);
    return format_score(lower) + '~' + format_score(upper);
}

UTF8Parser::UTF8Parser(char const *s) : str(s), at(s) {}

std::string UTF8Parser::trunc_string(std::string const &str, uint32_t max) {
    UTF8Parser parser(str.c_str());
#ifdef USE_CODEPOINT_LEN
    for (uint32_t i = 0; i < max; ++i)
        if (parser.next_symbol() == 0) break;
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
    return *at++;
}

bool UTF8Parser::is_valid_utf8(std::string const &str) {
    uint32_t at = 0;
    while (at < str.size()) {
        uint8_t c = (uint8_t) str[at];
        if (c < 0x80) {
            ++at;
            continue;
        }
        uint32_t run_len;
        if (c >= 0xf0)
            run_len = 3;
        else if (c >= 0xe0)
            run_len = 2;
        else
            run_len = 1;
        if (at + run_len > str.size()) return false;
        for (uint32_t i = 0; i < run_len; ++i, ++at)
            if (((uint8_t) str[at]) < 0x80) return false;
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
