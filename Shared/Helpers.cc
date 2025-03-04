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

LerpFloat::LerpFloat() {
    value = lerp_value = 0;
    touched = 0;
}

void LerpFloat::operator=(float v) {
    value = v;
    if (!touched) {
        lerp_value = v;
        touched = 1;
    }
}

LerpFloat::operator float() const {
    return lerp_value;
}

void LerpFloat::step(float amt) {
    lerp_value = lerp(lerp_value, value, amt);
}