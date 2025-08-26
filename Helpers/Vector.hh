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
    Vector operator+(Vector const &) const;
    Vector operator-(Vector const &) const;
    Vector operator*(float) const;
    void set(float, float);
    float magnitude() const;
    float angle() const;
    Vector &normalize();
    Vector &set_magnitude(float);
    Vector &unit_normal(float);
};