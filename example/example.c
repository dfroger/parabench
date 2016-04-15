#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#include "parabench.h"

#define KILO 1000
#define MEGA KILO*KILO

int
main(int argc, char **argv)
{
    // Initialize MPI.
    MPI_Init(&argc, &argv);
    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // Parser command line.
    long problem_size;
    bool debug_mode;
    if (!parabench_parse_command_line(argc, argv, &problem_size, &debug_mode)) {
        MPI_Finalize();
        exit(0);
    }

    // Compute time to sleep.
    long milliseconds = problem_size / mpi_size;
    struct timespec request, remaining;

    ParabenchStopwatch sw;
    sw.mpi_rank = mpi_rank;

    // Sleep.
    sw.name = "foo";
    parabench_start(&sw);
    request.tv_sec  = 0.75 * milliseconds / KILO;
    request.tv_nsec = 0.75 * (milliseconds % KILO) * MEGA;
    fprintf(stderr, "Rank: %d, sleeping for %lld.%09ld\n", mpi_rank,
            (long long) request.tv_sec, request.tv_nsec);
    request.tv_sec  = 0.75 * milliseconds / KILO;
    nanosleep(&request, &remaining);
    parabench_stop(&sw);
    parabench_report(&sw);

    // Sleep.
    sw.name = "bar";
    parabench_start(&sw);
    request.tv_sec  = 0.25 * milliseconds / KILO;
    request.tv_nsec = 0.25 * (milliseconds % KILO) * MEGA;
    fprintf(stderr, "Rank: %d, sleeping for %lld.%09ld\n", mpi_rank,
           (long long) request.tv_sec, request.tv_nsec);
    nanosleep(&request, &remaining);
    parabench_stop(&sw);
    parabench_report(&sw);

    // Finalize MPI.
    MPI_Finalize();

    return 0;
}
