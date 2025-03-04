#include <Shared/Vector.hh>

#include <cmath>

Vector::Vector() {}

Vector::Vector(Vector &o) : x(o.x) , y(o.y) {}

Vector::Vector(float _x, float _y) : x(_x), y(_y) {}

void Vector::set(float _x, float _y) {
    x = _x;
    y = _y;
}

Vector &Vector::operator=(Vector &o) {
    x = o.x;
    y = o.y;
    return *this;
}

Vector &Vector::operator+=(Vector &o) {
    x += o.x;
    y += o.y;
    return *this;
}

Vector &Vector::operator*=(float v) {
    x *= v;
    y *= v;
    return *this;
}

float Vector::magnitude() {
    return sqrtf(x * x + y * y);
}

float Vector::angle() {
    if (x == 0 && y == 0) return 0;
    return atan2f(y, x);
}

Vector &Vector::normalize() {
    if (x == 0 && y == 0) return *this;
    float mag = magnitude();
    x /= mag;
    y /= mag;
    return *this;
}

Vector &Vector::set_magnitude(float v) {
    normalize();
    x *= v;
    y *= v;
    return *this;
}

Vector &Vector::unit_normal(float a) {
    x = cosf(a);
    y = sinf(a);
    return *this;
}