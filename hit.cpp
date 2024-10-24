#include "hit.h"
#include <cmath>

bool hit_test(float x, float y, float z) {
    return (x * x * x * x - 2 * x * x * x) + 4 * (y * y + z * z) <= 0;
}

const float* get_axis_range() {
    static float axis_range[6];

    axis_range[0] = 0.0f;    // x_min
    axis_range[1] = 2.0f;    // x_max
    axis_range[2] = -0.65f;  // y_min
    axis_range[3] = 0.65f;   // y_max
    axis_range[4] = -0.65f;  // z_min
    axis_range[5] = 0.65f;   // z_max

    return axis_range;
}

