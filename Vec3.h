#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include <cstdlib>

#include "rtweekend.h"



class Vec3 {
public:
    Vec3() {}

    Vec3(float e0, float e1, float e2) {
            e[0] = e0;
            e[1] = e1;
            e[2] = e2;
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3 &operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float &operator[](int i) { return e[i]; }
    inline Vec3 &operator+=(const Vec3 &v2);
    inline Vec3 &operator-=(const Vec3 &v2);
    inline Vec3 &operator*=(const Vec3 &v2);
    inline Vec3 &operator/=(const Vec3 &v2);
    inline Vec3 &operator*=(const float t);
    inline Vec3 &operator/=(const float t);

    inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline float squaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline void makeUnitVector();

    float e[3];


    bool near_zero() const 
    {
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
    }
};



inline std::istream &operator>>(std::istream &is, Vec3 &t) {
is >> t.e[0] >> t.e[1] >> t.e[2];
return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vec3 &t) {
os << t.e[0] << " " << t.e[1] << " " << t.e[2];
return os;
}   
inline void Vec3::makeUnitVector() {
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}
inline Vec3 operator*(float t, const Vec3 &v) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline Vec3 operator/(Vec3 v, float t) {
    return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}
inline Vec3 operator*(const Vec3 &v, float t) {
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline Vec3 &Vec3::operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}
inline Vec3 &Vec3::operator*=(const Vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}
inline Vec3 &Vec3::operator/=(const Vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}
inline Vec3 &Vec3::operator-=(const Vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}
inline Vec3 &Vec3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline Vec3 &Vec3::operator/=(const float t) {
    float k = 1.0f / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}
inline Vec3 unitVector(Vec3 v) {
    return v / v.length();
}
inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.e[0] * v2.e[0]
    + v1.e[1] * v2.e[1]
    + v1.e[2] * v2.e[2];
}
inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
    v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
    v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}
inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vec3& v, const Vec3& n, float niOverNt, Vec3& refracted) {
    Vec3 uv = unitVector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - niOverNt * niOverNt * (1 - dt * dt);
    if(discriminant > 0) 
    {
        refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    return false;

}

inline float schlick(float cosine, float refractionIndex) {
    float r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline static Vec3 random() 
{
    return Vec3(random_double(), random_double(), random_double());
}

inline static Vec3 random(double min, double max) 
{
    return Vec3(random_double(min,max), random_double(min,max), random_double(min,max));
}

Vec3 random_in_unit_sphere() {
    while (true) {
        auto p = random(-1,1);
        if (p.squaredLength() >= 1) continue;
        return p;
    }
}

Vec3 random_unit_vector() {
    return unitVector(random_in_unit_sphere());
}

//Renvoie le vecteur random dans le sens de la normale
Vec3 random_in_hemisphere(const Vec3& normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vec3 Reflect(const Vec3& v, const Vec3& n) {
    return v - 2*dot(v,n)*n;
}

//Fonction pour donner un vecteur de réfraction (fonction mathématique)
Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squaredLength())) * n;
    return r_out_perp + r_out_parallel;
}

//Génère des  points dans un disque(pas de z du coup)
Vec3 random_in_unit_disk() {
    while (true) {
        auto p = Vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.squaredLength() >= 1) continue;
        return p;
    }
}


#endif