#pragma once

#include <cstdint>

struct xor_shift_random {
    uint32_t seed_a, seed_b;

    xor_shift_random(uint32_t seed_a, uint32_t seed_b) {
        this->seed_a = seed_a;
        this->seed_b = seed_b;
        for (int i = 0; i < 19; i++) next();
    }

    xor_shift_random(const xor_shift_random& other) {
        this->seed_a = other.seed_a;
        this->seed_b = other.seed_b;
        for (int i = 0; i < 19; i++) next();
    }

    uint32_t next() {
        uint32_t v = roll_2_7_3(seed_a, seed_b);
        seed_a = seed_b;
        seed_b = v;
        return v;
    }

    static uint32_t roll_2_7_3(uint32_t a, uint32_t b) {
        uint32_t t = a ^ (a << 2);
        uint32_t k = t ^ (t >> 7);
        return (b ^ (b >> 3)) ^ k;
    }

    // returns [0 .. 1) with 24 bits of randomness
    float next_float() {
        int bits24 = int(next() & 0xFFFFFF);
        return 0x1p-24f * float(bits24);
    }

    float next_float(float min, float max) {
        return min + (max - min) * next_float();
    }
};
