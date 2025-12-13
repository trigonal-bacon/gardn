#include <Helpers/Vector.hh>

#include <Helpers/Math.hh>

#include <cmath>

Vector::Vector() {}

Vector::Vector(Vector &o) : x(o.x) , y(o.y) {}

Vector::Vector(float _x, float _y) : x(_x), y(_y) {}

Vector Vector::rand(float magnitude) {
    float angle = frand() * 2 * M_PI;
    return { cosf(angle) * magnitude, sinf(angle) * magnitude };
}

void Vector::set(float _x, float _y) {
    x = _x;
    y = _y;
}


Vector &Vector::operator=(Vector const &o) {
    x = o.x;
    y = o.y;
    return *this;
}

Vector &Vector::operator+=(Vector const &o) {
    x += o.x;
    y += o.y;
    return *this;
}

Vector &Vector::operator-=(Vector const &o) {
    x -= o.x;
    y -= o.y;
    return *this;
}

Vector &Vector::operator*=(float v) {
    x *= v;
    y *= v;
    return *this;
}

Vector Vector::operator+(Vector const &v) const {
    return Vector{ x + v.x, y + v.y };
}

Vector Vector::operator-(Vector const &v) const {
    return Vector{ x - v.x, y - v.y };
}

Vector Vector::operator*(float v) const {
    return Vector{ x * v, y * v };
}

float Vector::magnitude() const {
    return sqrtf(x * x + y * y);
}

float Vector::angle() const {
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