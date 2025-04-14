#pragma once
#include "Object.h"
#include "helper_functions.h"

class Plane : public Object {
	Point3 Origin;
	Vec3 Normal;
	color PlaneColor;
	float albedo;
public:
	Plane(const Point3& o, const Vec3& n) : Origin(o), Normal(Normalized(n)), PlaneColor(color(1.0, 0, 0)), albedo(0.18) {}
	Plane(const Point3& o, const Vec3& n, const color& Color) : Origin(o), Normal(Normalized(n)), PlaneColor(Color), albedo(0.18) {}

	bool check_intersection(const Ray& ray, float& t) const override {
		const float Perpendicular_Dot = Dot_Product(ray.direction, Normal);
		const float Perpendicular_Distance = Dot_Product(Origin - ray.origin, Normal);
		if (Perpendicular_Dot > 1e-6 ) {
			t = Perpendicular_Distance / Perpendicular_Dot;
			if (t >= 0) {
				return true;
			}
		}

		t = MAX_RENDER_DISTANCE;

		return false;
	}

	const color& get_color() const override {
		return PlaneColor;
	}

	const Vec3& get_surface_normal(const Point3& Point_Hit) const override {
		return -Normal;
	}

	const float& get_albedo() const override {
		return albedo;
	}
};

//currently can only be seen when oriented away from the camera