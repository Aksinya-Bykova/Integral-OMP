#ifndef OMP_CUSTOM_PARSER_LIB_H
#define OMP_CUSTOM_PARSER_LIB_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <random>
#include <string>
#include <omp.h>
#include "hit.h"

struct Args {
    std::string input_file;
    std::string output_file;
    int threads = 0;

    Args(int argc, char* argv[]) {
        if (argc < 6 || argc > 7) {
            std::cerr << "Wrong input" << std::endl;
            exit(EXIT_FAILURE);
        }
        int threads_num_pos = -1;
        int type_pos = -1;
        for (int i = 1; i < argc; ++i) {
            if (std::string(argv[i]) == "--no-omp") {
                if (argc != 6) {
                    std::cerr << "Wrong input" << std::endl;
                    exit(EXIT_FAILURE);
                }
                type_pos = i;
            }
            if (std::string(argv[i]) == "--omp-threads") {
                if (argc != 7 || i == argc) {
                    std::cerr << "Wrong input" << std::endl;
                    exit(EXIT_FAILURE);
                }
                type_pos = i;
                if (std::string(argv[i + 1]) == "default") {
                    threads = omp_get_max_threads();
                    threads_num_pos = i + 1;
                } else {
                    const char* num_str = argv[i + 1];
                    for (const char* c = num_str; *c != '\0'; ++c) {
                        if (!std::isdigit(*c)) {
                            std::cerr << "Wrong input" << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    }
                    threads = std::stoi(num_str);
                    threads_num_pos = i + 1;
                }
            }
        }
        if (type_pos == -1) {
            std::cerr << "Wrong input" << std::endl;
            exit(EXIT_FAILURE);
        }
        if (threads_num_pos == -1 && threads != 0) {
            std::cerr << "Wrong input" << std::endl;
            exit(EXIT_FAILURE);
        }
        int input_file_pos = -1;
        int output_file_pos = -1;
        for (int i = 1; i < argc; ++i) {
            if (i == type_pos || i == threads_num_pos) {
                continue;
            }
            if (std::string(argv[i]) == "--input") {
                input_file = std::string(argv[i + 1]);
                input_file_pos = i + 1;
            }
            if (std::string(argv[i]) == "--output") {
                output_file = std::string(argv[i + 1]);
                output_file_pos = i + 1;
            }
        }
        if (input_file_pos == -1 || output_file_pos == -1) {
            std::cerr << "Wrong input" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
};

void read_input_file(const std::string& filename, long long& num_points) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    input_file >> num_points;
    input_file.close();
}

void write_output_file(const std::string& filename, float num_points) {
    std::ofstream file(filename, std::ios::app);

    if (file.is_open()) {
        file << num_points << std::endl;
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif //OMP_CUSTOM_PARSER_LIB_H
