#ifndef PARABENCH_H_INCLUDED
#define PARABENCH_H_INCLUDED

#include <stdbool.h>

typedef struct {
    int mpi_rank;
    const char* name;
    double start_time;
    double stop_time;
    double duration;
} ParabenchStopwatch;

bool
parabench_parse_command_line(int argc, char** argv,
                             long* problem_size, bool* debug_mode);

bool
parabench_start(ParabenchStopwatch* sw);

bool
parabench_stop(ParabenchStopwatch* sw);


bool
parabench_report(ParabenchStopwatch* sw);

#endif
