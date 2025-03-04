#pragma once

class Vector {
public:
    float x;
    float y;
    Vector();
    Vector(Vector &);
    Vector(float, float);
    Vector &operator=(Vector &);
    Vector &operator+=(Vector &);
    Vector &operator*=(float);
    void set(float, float);
    float magnitude();
    float angle();
    Vector &normalize();
    Vector &set_magnitude(float);
    Vector &unit_normal(float);
};