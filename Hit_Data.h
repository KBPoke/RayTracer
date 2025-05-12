#pragma once
#include <memory>
#include "Vec3.h"
#include "Object.h"

struct Hit_Data {
	const Ray ray;
	const float t;
	const Point3 Point_Hit;
	const Vec3 Hit_Normal;
	const std::shared_ptr<const Object> Object_Hit;

	Hit_Data(Ray r, float t, const Point3& PHit, const Vec3& HNorm, const std::shared_ptr<const Object>& ObjectHit) 
		: ray(r), t(t), Point_Hit(PHit), Hit_Normal(HNorm), Object_Hit(ObjectHit) {}
};