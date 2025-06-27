#pragma once

class Vector {
public:
    float x;
    float y;
    Vector();
    Vector(Vector &);
    Vector(float, float);
    static Vector rand(float);
    Vector &operator=(Vector const &);
    Vector &operator+=(Vector const &);
    Vector &operator-=(Vector const &);
    Vector &operator*=(float);
    Vector operator+(Vector const &);
    Vector operator-(Vector const &);
    Vector operator*(float);
    void set(float, float);
    float magnitude();
    float angle();
    Vector &normalize();
    Vector &set_magnitude(float);
    Vector &unit_normal(float);
};