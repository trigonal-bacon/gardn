#include <Shared/Helpers.hh>

#include <random>

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

RangeValue::RangeValue(double l, double u) {
    upper = u;
    lower = l;
}

RangeValue::RangeValue(double l) {
    lower = upper = l;
}

float RangeValue::get_single(float a) const {
    return lower + (upper - lower) * fclamp(a, 0, 1);
}

std::string RangeValue::to_string() const {
    if (lower == upper) return format_score(lower);
    return format_score(lower) + '~' + format_score(upper);
}

UTF8Parser::UTF8Parser(char const *s) : str(s), at(s) {}

std::string UTF8Parser::trunc_string(std::string const &str, uint32_t max) {
    UTF8Parser parser(str.c_str());
    uint32_t last = 0;
    while (last <= max) {
        if (parser.next_symbol() == 0) break;
        uint32_t len = parser.offset();
        if (len > max) break;
        last = len;
    }
    return str.substr(0, last);
}

char UTF8Parser::next_char() {
    if (*at == 0) return 0;
    return *at++;
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

uint32_t UTF8Parser::offset() const {
    return at - str;
}