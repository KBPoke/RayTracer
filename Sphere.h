#pragma once
#include "helper_functions.h"

class Sphere {
	Point3 centre;
	float radius, radius_square;
	color SphereColor;

public:
	Sphere(const Point3& c, const float r) : centre(c), radius(r), radius_square(r * r), SphereColor(color(1.0,0,0)) {}
	Sphere(const Point3& c, const float r, color Color) : centre(c), radius(r), radius_square(r* r), SphereColor(Color) {}

	bool check_intersection(const Ray& ray, float& t) const {
		Vec3 displacement = centre - ray.origin;
		float a = ray.direction.Norm_squared();
		float b = 2 * Dot_Product(ray.direction, displacement);
		float c = displacement.Norm_squared() - radius * radius;

		t = nearest_root(a, b, c);
		if (t >= MAX_RENDER_DISTANCE) { return false; }

		return true;
	}

	const color& return_color() const {
		return SphereColor;
	}
};