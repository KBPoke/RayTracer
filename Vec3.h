#pragma once
#include <cmath>
#include <iostream>
#include <cmath>

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vec3& operator=(const Vec3& other) {
        if (this == &other) {
            return *this;
        }

        x = other.x;
        y = other.y;
        z = other.z;

        return *this;
    }

    Vec3& operator+=(const Vec3& v) { //Cannot add to itself, but I don't think that's ever what you'd want to do over scalar doubling
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator+=(const Vec3&& v) { //Cannot add to itself, but I don't think that's ever what you'd want to do over scalar doubling
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3& operator*=(const float a) {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }

    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    Vec3 operator*(const float& a) const {
        return Vec3(a * x, a * y, a * z);
    }

    friend Vec3 operator*(const float a, const Vec3& v) {
        return v * a;
    }

    Vec3 operator/(const float a) const {
        return Vec3(x / a, y / a, z / a);
    }

    float Norm_squared() const {
        return x * x + y * y + z * z;
    }

    float Norm() const {
        return std::sqrt(Norm_squared());
    }

    friend std::ostream& operator<<(std::ostream& o, const Vec3& vec) {
        o << "{" << vec.z << "," << vec.y << "," << vec.z << "}\n";
        return o;
    }

    friend float Dot_Product(const Vec3& v, const Vec3& u) {
        return v.x * u.x + v.y * u.y + v.z * u.z;
    }

    friend Vec3 Cross_Product(const Vec3& v, const Vec3& u) {
        return Vec3(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
    }

    friend Vec3 Normalized(const Vec3& v) {
        const float InvNorm = 1 / v.Norm();
        return Vec3(v.x * InvNorm, v.y * InvNorm, v.z * InvNorm);
    }

    friend Vec3 Reflect(const Vec3& incident, const Vec3& normal) {
        return incident - 2 * normal * Dot_Product(incident, normal);
    }

    friend Vec3 Refract(const Vec3& incident, Vec3 normal, const float refraction_index) {
        float cosine_of_vector_angle = Dot_Product(incident, normal);
        float index_ratio = 1 / refraction_index;

        if (cosine_of_vector_angle < 0) {
            cosine_of_vector_angle = -cosine_of_vector_angle;
        }
        else {
            normal = -normal;
            index_ratio = refraction_index;
        }
        
        float k = 1 - index_ratio * index_ratio * (1 - cosine_of_vector_angle * cosine_of_vector_angle);
        if (k < 0) {
            return Reflect(incident, normal);
        }
        else return index_ratio * incident + (index_ratio * cosine_of_vector_angle - sqrt(k)) * normal;
    }

    friend float Fresnel_reflected_ratio(const Vec3& incident, Vec3 normal, const float refraction_index) {
        float cosine_of_vector_angle = Dot_Product(incident, normal);
        float index_ratio = 1 / refraction_index;

        if (cosine_of_vector_angle > 0) {
            index_ratio = refraction_index;
        }

        float sine_of_refracted_vector_angle = index_ratio * std::sqrtf(std::max(0.f, 1 - cosine_of_vector_angle * cosine_of_vector_angle));
        // Total internal reflection
        if (sine_of_refracted_vector_angle >= 1) {
            return 1;
        }
        else {
            float cosine_of_refracted_vector_angle = std::sqrtf(std::max(0.f, 1 - sine_of_refracted_vector_angle * sine_of_refracted_vector_angle));
            cosine_of_vector_angle = std::fabsf(cosine_of_vector_angle);
            float Rs = ((refraction_index * cosine_of_vector_angle) - cosine_of_refracted_vector_angle) / ((refraction_index * cosine_of_vector_angle) + cosine_of_refracted_vector_angle);
            float Rp = (cosine_of_vector_angle - (refraction_index * cosine_of_refracted_vector_angle)) / (cosine_of_vector_angle + (refraction_index * cosine_of_refracted_vector_angle));
            return (Rs * Rs + Rp * Rp) / 2;
        }
        return 0;
    }
};

using Point3 = Vec3;
using color = Vec3;