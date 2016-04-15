#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

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
parabench_parse_command_line(int argc, char** argv,
                             long* problem_size, bool* debug_mode)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s PROBLEM_SIZE DEBUG_MODE\n", argv[0]);
        return false;
    }

    long long_debug_mode;

    if (!str2long(argv[1], problem_size)) {
        fprintf(stderr,
                "ERROR: PROBLEM_SIZE: <%s> cannot be converted to integer.\n",
                argv[1]);
        return false;
    }

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

    return true;
}

bool
parabench_start(ParabenchStopwatch* sw)
{
    sw->start_time = get_wall_time();
}

bool
parabench_stop(ParabenchStopwatch* sw)
{
    sw->stop_time = get_wall_time();
    sw->duration = sw->stop_time - sw->start_time;
}

bool
parabench_report(ParabenchStopwatch* sw)
{
    printf("rank %d %s %.7f\n", sw->mpi_rank, sw->name, sw->duration);
}
