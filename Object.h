#pragma once
#include "Ray.h"

class Object {
public:
	virtual ~Object() = default;

	virtual bool check_intersection(const Ray& ray, float& t) const = 0;
	virtual const color& get_color() const = 0;
};
