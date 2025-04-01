#include "helper_functions.h"
#include <cmath>

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