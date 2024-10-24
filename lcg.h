#pragma once

#include <cstdint>

struct lcg_random {
    uint32_t state;

    lcg_random(uint32_t seed) : state(seed) {}

    uint32_t next() {
        state = 1664525 * state + 1013904223;
        return state;
    }

    float next_float() {
        return (next() >> 8) * 0x1p-24f;
    }

    float next_float(float min, float max) {
        return min + (max - min) * next_float();
    }
};
