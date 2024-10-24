#include <iostream>
#include <fstream>
#include <cstdlib>
#include "parser_lib.h"
//#include "research/research_best.h"
#include "monte_carlo.h"

void calculate(const Args& args, long long num_points) {
    double start_time, end_time;
    double volume = 0;

    if (args.threads == 0) {
        start_time = omp_get_wtime();
        volume = result<lcg_random>(num_points, get_axis_range());
        end_time = omp_get_wtime();
    } else {
        start_time = omp_get_wtime();
        volume = result<lcg_random>(num_points, get_axis_range(), args.threads,
                                        type_schedule::static_schedule);
        end_time = omp_get_wtime();
    }

    fprintf(stdout, "Time (%i thread(s)): %g ms\n", args.threads, (end_time - start_time) * 1000);

    write_output_file(args.output_file, volume);
}

int main(int argc, char* argv[]) {
    Args args(argc, argv);

    long long num_points;
    read_input_file(args.input_file, num_points);

//    search_optimal_without_chunks(num_points);

    calculate(args, num_points);
    return 0;
}
