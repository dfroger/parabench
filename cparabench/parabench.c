#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "mpi.h"

#include "parabench.h"

bool
str2long(char* str, long* value)
{
    if (str[0] == '\0') return false;
    char* endptr;
    *value = strtol(str, &endptr, 10);
    return endptr[0] == '\0';
}

double
get_wall_time()
{
    double micro = 0.000001;
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * micro;
}

bool
prb_parse_command_line(int argc, char** argv,
                       long* problem_size, bool* debug_mode,
                       char** output_filepath)
{
    // Check arguments number.
    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s PROBLEM_SIZE DEBUG_MODE OUTPUT_FILEPATH\n",
                argv[0]);
        return false;
    }


    // Read problem size.
    if (!str2long(argv[1], problem_size)) {
        fprintf(stderr,
                "ERROR: PROBLEM_SIZE: <%s> cannot be converted to integer.\n",
                argv[1]);
        return false;
    }

    // Read debug mode.
    long long_debug_mode;
    if (!str2long(argv[2], &long_debug_mode)) {
        fprintf(stderr,
                "ERROR: DEBUG_MODE: <%s> cannot be converted to integer.\n",
                argv[2]);
        return false;
    }
    if (long_debug_mode == 0)
        *debug_mode = false;
    else if (long_debug_mode == 1)
        *debug_mode = true;
    else {
        fprintf(stderr, "ERROR: DEBUG_MODE must be 0 or 1\n");
        return false;
    }

    // Read output file path.
    *output_filepath = argv[3];

    return true;
}

void
prb_partition_index(int mpi_size, int mpi_rank,
                    size_t problem_size, size_t* i0, size_t* i1)
{
    *i0 = problem_size *  mpi_rank / mpi_size;
    *i1 = problem_size * (mpi_rank+1) / mpi_size;
}

prb_stopwatch_t*
prb_stopwatch_new(int noperations)
{
    prb_stopwatch_t* sw = malloc(sizeof(prb_stopwatch_t));
    sw->names = malloc(sizeof(char*)*noperations);
    sw->start_times = malloc(sizeof(double)*noperations);
    sw->stop_times = malloc(sizeof(double)*noperations);
    sw->durations = malloc(sizeof(double)*noperations);
}

void
prb_stopwatch_free(prb_stopwatch_t* sw)
{
    free(sw->names);
    free(sw->start_times);
    free(sw->stop_times);
    free(sw->durations);
    free(sw);
}

bool
prb_stopwatch_start(prb_stopwatch_t* sw, int op)
{
    sw->start_times[op] = get_wall_time();
}

bool
prb_stopwatch_stop(prb_stopwatch_t* sw, int op)
{
    sw->stop_times[op] = get_wall_time();
    sw->durations[op] = sw->stop_times[op] - sw->start_times[op];
}

void
csv_write_header(FILE* csv, char** operation_names, int noperations)
{
    // Write operations names in header line.
    int iop;
    for (iop = 0 ; iop < noperations-1 ; iop++) {
        fprintf(csv, "%s, ", operation_names[iop]);
    }
    fprintf(csv, "%s\n", operation_names[noperations-1]);
}

void
csv_write_durations(FILE* csv, double* durations, int noperations)
{
    // Write on one line operation durations.
    int iop;
    for (iop = 0 ; iop < noperations-1 ; iop++) {
        fprintf(csv, "%.7f, ", durations[iop]);
    }
    fprintf(csv, "%.7f\n", durations[noperations-1]);
}

void
prb_stopwatch_write_csv(prb_stopwatch_t* sw, const char* filepath,
                        int noperations, char** operation_names,
                        MPI_Comm comm, int mpi_master)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int tag=0;

    if (rank==mpi_master) {
        FILE* csv = fopen(filepath, "w");
        double* other_durations = malloc(sizeof(double)*noperations);
        MPI_Status status;

        csv_write_header(csv, operation_names, noperations);

        // For each rank, write on one line operation durations.
        int other_rank;
        for (other_rank = 0 ; other_rank < size ; other_rank++) {
            if (other_rank == rank) {
                csv_write_durations(csv, sw->durations, noperations);
            } else {
                MPI_Recv(other_durations, noperations, MPI_DOUBLE,
                         other_rank, tag, comm, &status);
                csv_write_durations(csv, other_durations, noperations);
            }
        }

        fclose(csv);
        free(other_durations);
    } else {
        MPI_Send(sw->durations, noperations, MPI_DOUBLE, mpi_master, tag, comm);
    }
};
