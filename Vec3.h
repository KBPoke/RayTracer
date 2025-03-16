#pragma once

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}


    Vec3& operator+=(const Vec3& v) { //Cannot add to itself, but I don't think that's ever what you'd want to do over scalar doubling
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
};

float Dot_Product(const Vec3& v, const Vec3& u) {
    return v.x * u.x + v.y * u.y + v.z * u.z;
}

Vec3 Cross_Product(const Vec3& v, const Vec3& u) {
    return Vec3(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
}

Vec3 Normalized(const Vec3& v) {
    const float InvNorm = 1 / v.Norm();
    return Vec3(v.x * InvNorm, v.y * InvNorm, v.z * InvNorm);
}

using Point3 = Vec3;