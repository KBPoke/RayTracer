#pragma once
#include <cstdlib>

#define MAX_RENDER_DISTANCE 1e9

float nearest_root(float a, float b, float c);

const float PI = 3.141592653589793;

enum ObjectType {
	Diffuse,
	Reflective,
	Refractive,
	Fresnel
};