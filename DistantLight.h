#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "Vec3.h"

class DistantLight {
	Vec3 Direction;
	color Color;
	float Intensity;

public:
	DistantLight(const Vec3& dir) : Direction(Normalized(dir)), Color(color(1.0, 1.0, 1.0)), Intensity(5) {}
	DistantLight(const Vec3& dir, const color& col, float intensity) : Direction(Normalized(dir)), Color(col), Intensity(intensity) {}

	bool check_direct_Lighting(const Point3& Point_Hit, const Vec3& Normal_Hit, const std::vector<std::shared_ptr<Object>>& ObjectList) const {
		const float bias = 0.001;
		const Ray RayToLightSource(Point_Hit + bias * Normal_Hit, -Direction);
		float temp = 0;

		for (const std::shared_ptr<const Object> object : ObjectList) {
			if (object->check_intersection(RayToLightSource, temp) == true) {
				return false;
			}
		}

		return true;
	}

	const color& get_color() const {
		return Color;
	}

	const Vec3& get_direction() const {
		return Direction;
	}

	const float get_intensity() const {
		return Intensity;
	}
};