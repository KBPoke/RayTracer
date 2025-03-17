#pragma once

class Sphere {
	Point3 centre;
	float radius, radius_square;

public:
	Sphere(const Point3& c, const float r) : centre(c), radius(r), radius_square(r * r) {}

	bool check_intersection(const Ray& ray, float& t) const {
		Vec3 displacement = centre - ray.get_origin();
		float a = ray.get_direction().Norm_squared();
		float b = 2 * Dot_Product(ray.get_direction(), displacement);
		float c = displacement.Norm_squared() - radius * radius;

		float t = nearest_root(a, b, c);
		if (t == MAX_RENDER_DISTANCE) { return false; }

		return true;
	}
};