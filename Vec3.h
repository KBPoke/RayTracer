#pragma once

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}


    Vec3& operator+=(const Vec3& other) { //Cannot add to itself, but I don't think that's ever what you'd want to do over scalar doubling
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& operator*=(float a) {
        x *= a;
        y *= a;
        z *= a;
        return *this;
    }


    Vec3 Normalized() const {
        const float InvNorm = 1 / Norm();
        Vec3 ret = { x * InvNorm, y * InvNorm, z * InvNorm };
        return ret;
    }

    float Norm_squared() const {
        return x * x + y * y + z * z;
    }

    float Norm() const {
        return std::sqrt(Norm_squared());
    }
};