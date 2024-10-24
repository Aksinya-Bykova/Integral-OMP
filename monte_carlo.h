#ifndef OMP_CUSTOM_MONTE_CARLO_H
#define OMP_CUSTOM_MONTE_CARLO_H

#include <omp.h>
#include <cstdlib>
#include "xor_shift_random.h"
#include "mersenne.h"
#include "lcg.h"
#include "hit.h"

// для проверки без симейка
constexpr size_t CACHE_LINE_SIZE = 64;

struct alignas(CACHE_LINE_SIZE) PointData {
    float x;
    float y;
    float z;
};

enum class type_schedule{
    static_schedule,
    dynamic_schedule
};

float calculate_volume(const long long num_points, long long num_inside, const float* axis_range) {
    float volume_of_region = (axis_range[1] - axis_range[0]) *
                             (axis_range[3] - axis_range[2]) *
                             (axis_range[5] - axis_range[4]);
    return volume_of_region * (static_cast<float>(num_inside) / num_points);
}

template<typename RandType>
long long count_points_inside(const long long num_points, const float* axis_range, RandType& rt) {
    PointData point = {
            rt.next_float(axis_range[0], axis_range[1]),
            rt.next_float(axis_range[2], axis_range[3]),
            rt.next_float(axis_range[4], axis_range[5])
    };

    long long count_inside = 0;

    for (long long i = 0; i < num_points; ++i) {
        if (hit_test(point.x, point.y, point.z)) {
            ++count_inside;
        }
        if (i % 3 == 0) { point.x = rt.next_float(axis_range[0], axis_range[1]); }
        if (i % 3 == 1) { point.y = rt.next_float(axis_range[2], axis_range[3]); }
        if (i % 3 == 2) { point.z = rt.next_float(axis_range[4], axis_range[5]); }
    }
    return count_inside;
}

template<typename RandType>
long long count_points_inside_dynamic(const long long num_points, const float* axis_range, RandType rt, int threads) {
    long long total_count_inside = 0;

#pragma omp parallel num_threads(threads)
    {
        long long local_count_inside = 0;

        RandType local_rt(rt);
        PointData point = {
                local_rt.next_float(axis_range[0], axis_range[1]),
                local_rt.next_float(axis_range[2], axis_range[3]),
                local_rt.next_float(axis_range[4], axis_range[5])
        };

#pragma omp for schedule(dynamic)
        for (long long i = 0; i < num_points; ++i) {
            if (hit_test(point.x, point.y, point.z)) {
                ++local_count_inside;
            }
            if (i % 3 == 0) { point.x = local_rt.next_float(axis_range[0], axis_range[1]); }
            if (i % 3 == 1) { point.y = local_rt.next_float(axis_range[2], axis_range[3]); }
            if (i % 3 == 2) { point.z = local_rt.next_float(axis_range[4], axis_range[5]); }
        }

#pragma omp atomic
        total_count_inside += local_count_inside;
    }

    return total_count_inside;
}

template<typename RandType>
long long count_points_inside_static(const long long num_points, const float* axis_range, RandType rt, int threads) {
    long long total_count_inside = 0;

#pragma omp parallel num_threads(threads)
    {
        long long local_count_inside = 0;

        RandType local_rt(rt);
        PointData point = {
                local_rt.next_float(axis_range[0], axis_range[1]),
                local_rt.next_float(axis_range[2], axis_range[3]),
                local_rt.next_float(axis_range[4], axis_range[5])
        };

#pragma omp for schedule(static)
        for (long long i = 0; i < num_points; ++i) {
            if (hit_test(point.x, point.y, point.z)) {
                ++local_count_inside;
            }
            if (i % 3 == 0) { point.x = local_rt.next_float(axis_range[0], axis_range[1]); }
            if (i % 3 == 1) { point.y = local_rt.next_float(axis_range[2], axis_range[3]); }
            if (i % 3 == 2) { point.z = local_rt.next_float(axis_range[4], axis_range[5]); }
        }

#pragma omp atomic
        total_count_inside += local_count_inside;
    }

    return total_count_inside;
}

template<typename RandType>
long long count_points_inside_dynamic(const long long num_points, const float* axis_range, RandType rt, int threads, int chunk_size) {
    long long total_count_inside = 0;

#pragma omp parallel num_threads(threads)
    {
        long long local_count_inside = 0;

        RandType local_rt(rt);
        PointData point = {
                local_rt.next_float(axis_range[0], axis_range[1]),
                local_rt.next_float(axis_range[2], axis_range[3]),
                local_rt.next_float(axis_range[4], axis_range[5])
        };

#pragma omp for schedule(dynamic, chunk_size)
        for (long long i = 0; i < num_points; ++i) {
            if (hit_test(point.x, point.y, point.z)) {
                ++local_count_inside;
            }
            if (i % 3 == 0) { point.x = local_rt.next_float(axis_range[0], axis_range[1]); }
            if (i % 3 == 1) { point.y = local_rt.next_float(axis_range[2], axis_range[3]); }
            if (i % 3 == 2) { point.z = local_rt.next_float(axis_range[4], axis_range[5]); }
        }

#pragma omp atomic
        total_count_inside += local_count_inside;
    }

    return total_count_inside;
}

template<typename RandType>
long long count_points_inside_static(const long long num_points, const float* axis_range, RandType rt, int threads, int chunk_size) {
    long long total_count_inside = 0;

#pragma omp parallel num_threads(threads)
    {
        long long local_count_inside = 0;

        RandType local_rt(rt);
        PointData point = {
                local_rt.next_float(axis_range[0], axis_range[1]),
                local_rt.next_float(axis_range[2], axis_range[3]),
                local_rt.next_float(axis_range[4], axis_range[5])
        };

#pragma omp for schedule(static, chunk_size)
        for (long long i = 0; i < num_points; ++i) {
            if (hit_test(point.x, point.y, point.z)) {
                ++local_count_inside;
            }
            if (i % 3 == 0) { point.x = local_rt.next_float(axis_range[0], axis_range[1]); }
            if (i % 3 == 1) { point.y = local_rt.next_float(axis_range[2], axis_range[3]); }
            if (i % 3 == 2) { point.z = local_rt.next_float(axis_range[4], axis_range[5]); }
        }

#pragma omp atomic
        total_count_inside += local_count_inside;
    }

    return total_count_inside;
}

template<typename RandType>
float result(const long long num_points, const float* axis_range, int threads, const type_schedule& ts) {
    uint32_t seed = static_cast<uint32_t>(std::random_device{}());
    RandType rt(seed);

    long long num_inside = 0;

    if (ts == type_schedule::dynamic_schedule) {
        num_inside = count_points_inside_dynamic<RandType>(num_points, axis_range, rt, threads);
    } else if (ts == type_schedule::static_schedule) {
        num_inside = count_points_inside_static<RandType>(num_points, axis_range, rt, threads);
    }
    return calculate_volume(num_points, num_inside, axis_range);
}

template<>
float result<xor_shift_random>(const long long num_points, const float* axis_range, int threads, const type_schedule& ts) {
    uint32_t seed_a = static_cast<uint32_t>(std::random_device{}());
    uint32_t seed_b = static_cast<uint32_t>(std::random_device{}());
    xor_shift_random rt(seed_a, seed_b);

    long long num_inside = 0;

    if (ts == type_schedule::dynamic_schedule) {
        num_inside = count_points_inside_dynamic<xor_shift_random>(num_points, axis_range, rt, threads);
    } else if (ts == type_schedule::static_schedule) {
        num_inside = count_points_inside_static<xor_shift_random>(num_points, axis_range, rt, threads);
    }

    return calculate_volume(num_points, num_inside, axis_range);
}

template<typename RandType>
float result(const long long num_points, const float* axis_range, int threads, const type_schedule& ts, int chunk_size) {
    uint32_t seed = static_cast<uint32_t>(std::random_device{}());
    RandType rt(seed);

    long long num_inside = 0;

    if (ts == type_schedule::dynamic_schedule) {
        num_inside = count_points_inside_dynamic<RandType>(num_points, axis_range, rt, threads, chunk_size);
    } else if (ts == type_schedule::static_schedule) {
        num_inside = count_points_inside_static<RandType>(num_points, axis_range, rt, threads, chunk_size);
    }
    return calculate_volume(num_points, num_inside, axis_range);
}

template<>
float result<xor_shift_random>(const long long num_points, const float* axis_range, int threads, const type_schedule& ts,
                               int chunk_size) {
    uint32_t seed_a = static_cast<uint32_t>(std::random_device{}());
    uint32_t seed_b = static_cast<uint32_t>(std::random_device{}());
    xor_shift_random rt(seed_a, seed_b);

    long long num_inside = 0;

    if (ts == type_schedule::dynamic_schedule) {
        num_inside = count_points_inside_dynamic<xor_shift_random>(num_points, axis_range, rt, threads, chunk_size);
    } else if (ts == type_schedule::static_schedule) {
        num_inside = count_points_inside_static<xor_shift_random>(num_points, axis_range, rt, threads, chunk_size);
    }

    return calculate_volume(num_points, num_inside, axis_range);
}

template<typename RandType>
float result(const long long num_points, const float* axis_range) {
    uint32_t seed = static_cast<uint32_t>(std::random_device{}());
    RandType rt(seed);

    long long num_inside = count_points_inside<RandType>(num_points, axis_range, rt);
    return calculate_volume(num_points, num_inside, axis_range);
}

template<>
float result<xor_shift_random>(const long long num_points, const float* axis_range) {
    uint32_t seed_a = static_cast<uint32_t>(std::random_device{}());
    uint32_t seed_b = static_cast<uint32_t>(std::random_device{}());
    xor_shift_random rt(seed_a, seed_b);

    long long num_inside = count_points_inside<xor_shift_random>(num_points, axis_range, rt);

    return calculate_volume(num_points, num_inside, axis_range);
}

#endif //OMP_CUSTOM_MONTE_CARLO_H
