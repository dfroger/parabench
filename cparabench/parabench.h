#ifndef PARABENCH_H_INCLUDED
#define PARABENCH_H_INCLUDED

#include <stdbool.h>

typedef struct {
    char** names;
    double* start_times;
    double* stop_times;
    double* durations;
} prb_stopwatch_t;

bool
prb_parse_command_line(int argc, char** argv,
                       long* problem_size, bool* debug_mode,
                       char** output_filepath);

prb_stopwatch_t*
prb_stopwatch_new(int noperations);

void
prb_stopwatch_free(prb_stopwatch_t* sw);

bool
prb_stopwatch_start(prb_stopwatch_t* sw, int operation);

bool
prb_stopwatch_stop(prb_stopwatch_t* sw, int operation);

void
prb_stopwatch_write_csv(prb_stopwatch_t* sw, const char* filepath,
                        int noperations, char** operation_names,
                        MPI_Comm comm, int mpi_master);

#endif
