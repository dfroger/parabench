#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"
#include "parabench.h"

#define MEGA 1000000
#define GIGA 1000000000

int
main(int argc, char **argv)
{
    // Initialize MPI.
    MPI_Init(&argc, &argv);
    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // Parser command line.
    long milliseconds;
    bool debug_mode;
    char* output_filepath;
    if (!prb_parse_command_line(argc, argv, &milliseconds, &debug_mode, 
                                &output_filepath)) {
        MPI_Finalize();
        exit(0);
    }

    int noperations = 2;
    enum operations {FOO, BAR};
    char* operation_names[2] = {"foo", "bar"};

    // Compute time to sleep.
    long nanoseconds = (milliseconds*MEGA) / mpi_size;
    long nanoseconds_foo = 0.75 * nanoseconds;
    long nanoseconds_bar = 0.25 * nanoseconds;
    struct timespec request, remaining;

    prb_stopwatch_t* sw = prb_stopwatch_new(noperations);

    // Sleep.
    prb_stopwatch_start(sw, FOO);
    request.tv_sec  = nanoseconds_foo / GIGA;
    request.tv_nsec = nanoseconds_foo % GIGA;
    fprintf(stderr, "Rank: %d, sleeping for %lld.%09ld\n", mpi_rank,
            (long long) request.tv_sec, request.tv_nsec);
    nanosleep(&request, &remaining);
    prb_stopwatch_stop(sw, FOO);

    // Sleep.
    prb_stopwatch_start(sw, BAR);
    request.tv_sec  = nanoseconds_bar / GIGA;
    request.tv_nsec = nanoseconds_bar % GIGA;
    fprintf(stderr, "Rank: %d, sleeping for %lld.%09ld\n", mpi_rank,
           (long long) request.tv_sec, request.tv_nsec);
    nanosleep(&request, &remaining);
    prb_stopwatch_stop(sw, BAR);

    prb_stopwatch_write_csv(sw, output_filepath,
                            noperations, operation_names,
                            MPI_COMM_WORLD, 0);

    prb_stopwatch_free(sw);

    // Finalize MPI.
    MPI_Finalize();

    return 0;
}
