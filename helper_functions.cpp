#include <cmath>

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

color ray_color_background(const Ray& r) {
	Vec3 unit_direction = Normalized(r.get_direction());
	auto a = 0.5 * (unit_direction.y + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.5, 0.3);
}