#pragma once
#include "Ray.h"

class Object {
public:
	virtual ~Object() = default;

	virtual bool check_intersection(const Ray& ray, float& t) const = 0;
	virtual const color& get_color() const = 0;
	virtual const Vec3 get_surface_normal(const Point3& Point_Hit) const = 0;
	virtual const float& get_albedo() const = 0;
};
