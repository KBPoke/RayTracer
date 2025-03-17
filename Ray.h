#pragma once

class Ray {
	Point3 origin;
	Vec3 direction;

public:
	Ray() {}
	Ray(const Point3& orig, const Vec3& direct) : origin(orig), direction(Normalized(direct)) {}

	const Point3& get_origin() const { return origin; }
	const Vec3& get_direction() const { return direction; }

	Point3 delta_v(const float& t) const {
		return origin + t * direction;
	}
};