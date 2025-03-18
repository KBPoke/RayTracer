#pragma once

#define MAX_RENDER_DISTANCE 1e9

float nearest_root(float a, float b, float c) {
    float delta = b * b - 4 * a * c;

    if (delta < 0) {
        return MAX_RENDER_DISTANCE;
    }

    int sign;
    if (b < 0) { sign = -1; }
    else sign = 1;

    float x0, x1;

    x0 = (-b - sign * std::sqrt(delta)) / (2 * a);
    x1 = (2 * c) / (-b - sign * std::sqrt(delta));

    float ret = MAX_RENDER_DISTANCE;
    if (x0 > 0) {
        ret = x0;
    }
    if (x1 > 0 && x1 < x0) {
        ret = x1;
    }

    return ret;
}

class Sphere {
	Point3 centre;
	float radius, radius_square;

public:
	Sphere(const Point3& c, const float r) : centre(c), radius(r), radius_square(r * r) {}

	bool check_intersection(const Ray& ray, float& t) const {
		Vec3 displacement = centre - ray.origin;
		float a = ray.direction.Norm_squared();
		float b = 2 * Dot_Product(ray.direction, displacement);
		float c = displacement.Norm_squared() - radius * radius;

		t = nearest_root(a, b, c);
		if (t == MAX_RENDER_DISTANCE) { return false; }

		return true;
	}
};

