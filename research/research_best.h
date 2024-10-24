#ifndef OMP_CUSTOM_RESEARCH_BEST_H
#define OMP_CUSTOM_RESEARCH_BEST_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <omp.h>
#include <string>
#include "../monte_carlo.h"

using json = nlohmann::json;

struct Config {
    std::string random_type;
    int omp_include;
    int threads;
    std::string schedule_type;
    int chunk_size;
};

Config parseConfig(const std::string& filename) {
    Config config;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file");
    }

    json j;
    file >> j;

    config.random_type = j["random-type"].get<std::string>();
    config.omp_include = j["omp-include"].get<int>();
    config.threads = j["threads"].get<int>();
    config.schedule_type = j["schedule-type"].get<std::string>();
    config.chunk_size = j["chunk-size"].get<int>();

    return config;
}

void search_xor_shift(double& start_time, double& end_time, long long num_points, float& volume, const Config& config) {
    if (config.omp_include == 0) {
        start_time = omp_get_wtime();
        volume = result<xor_shift_random>(num_points, get_axis_range());
        end_time = omp_get_wtime();
    } else {
        if (config.schedule_type == "dynamic") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<xor_shift_random>(num_points, get_axis_range(), config.threads,
                                                  type_schedule::dynamic_schedule,
                                                  config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<xor_shift_random>(num_points, get_axis_range(), config.threads,
                                                  type_schedule::dynamic_schedule);
                end_time = omp_get_wtime();
            }
        }
        if (config.schedule_type == "static") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<xor_shift_random>(num_points, get_axis_range(), config.threads,
                                                  type_schedule::static_schedule,
                                                  config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<xor_shift_random>(num_points, get_axis_range(), config.threads,
                                                  type_schedule::static_schedule);
                end_time = omp_get_wtime();
            }
        }
    }
}

void search_mersenne(double& start_time, double& end_time, long long num_points, float& volume, const Config& config) {
    if (config.omp_include == 0) {
        start_time = omp_get_wtime();
        volume = result<mt19937_random>(num_points, get_axis_range());
        end_time = omp_get_wtime();
    } else {
        if (config.schedule_type == "dynamic") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<mt19937_random>(num_points, get_axis_range(), config.threads,
                                                type_schedule::dynamic_schedule,
                                                config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<mt19937_random>(num_points, get_axis_range(), config.threads,
                                                type_schedule::dynamic_schedule);
                end_time = omp_get_wtime();
            }
        }
        if (config.schedule_type == "static") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<mt19937_random>(num_points, get_axis_range(), config.threads,
                                                type_schedule::static_schedule,
                                                config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<mt19937_random>(num_points, get_axis_range(), config.threads,
                                                type_schedule::static_schedule);
                end_time = omp_get_wtime();
            }
        }
    }
}

void search_lcg(double& start_time, double& end_time, long long num_points, float& volume, const Config& config) {
    if (config.omp_include == 0) {
        start_time = omp_get_wtime();
        volume = result<lcg_random>(num_points, get_axis_range());
        end_time = omp_get_wtime();
    } else {
        if (config.schedule_type == "dynamic") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<lcg_random>(num_points, get_axis_range(), config.threads,
                                            type_schedule::dynamic_schedule,
                                            config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<lcg_random>(num_points, get_axis_range(), config.threads,
                                            type_schedule::dynamic_schedule);
                end_time = omp_get_wtime();
            }
        }
        if (config.schedule_type == "static") {
            if (config.chunk_size > 0) {
                start_time = omp_get_wtime();
                volume = result<lcg_random>(num_points, get_axis_range(), config.threads,
                                            type_schedule::static_schedule,
                                            config.chunk_size);
                end_time = omp_get_wtime();
            } else {
                start_time = omp_get_wtime();
                volume = result<lcg_random>(num_points, get_axis_range(), config.threads,
                                            type_schedule::static_schedule);
                end_time = omp_get_wtime();
            }
        }
    }
}

void search_optimal_without_chunks(long long num_points) {
    Config config = parseConfig("../research/config.json");

    std::string filename = "../research/data.txt";
    std::ofstream outFile;
    outFile.open(filename, std::ios::app);

    std::cout << "Random Type: " << config.random_type << std::endl;
    std::cout << "OMP Include: " << config.omp_include << std::endl;
    std::cout << "Threads: " << config.threads << std::endl;
    std::cout << "Schedule Type: " << config.schedule_type << std::endl;
    std::cout << "Chunk Size: " << config.chunk_size << std::endl;

    float volume = 0;

    double start_time, end_time;

    if (config.random_type == "xor-shift") {
        search_xor_shift(start_time, end_time, num_points, volume, config);
    }
    if (config.random_type == "mers") {
        search_mersenne(start_time, end_time, num_points, volume, config);
    }
    if (config.random_type == "lcg") {
        search_lcg(start_time, end_time, num_points, volume, config);
    }

    outFile << (end_time - start_time) * 1000 << std::endl;

    outFile.close();
}

#endif //OMP_CUSTOM_RESEARCH_BEST_H
