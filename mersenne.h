#pragma once

#include <random>

struct mt19937_random {
    std::mt19937 engine;

    mt19937_random(uint32_t seed) : engine(seed) {}

    float next_float(float min, float max) {
        return std::uniform_real_distribution<float>(min, max)(engine);
    }
};

