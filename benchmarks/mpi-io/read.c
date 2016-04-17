#include <stdio.h>
#include <stdlib.h>

#include "tool.h"

#include "mpi.h"
#include "parabench.h"

int
main( int argc, char **argv )
{
    // Initialize MPI.
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parser command line.
    long problem_size;
    bool debug_mode;
    char* output_filepath;
    if (!prb_parse_command_line(argc, argv, &problem_size, &debug_mode, 
                                &output_filepath)) {
        MPI_Finalize();
        exit(0);
    }

    long array_size = problem_size *  problem_size;

    // Compute partition index.
    size_t i0, i1;
    prb_partition_index(size, rank, array_size, &i0, &i1);

    // Allocate data partition.
    size_t partition_size = i1 - i0;
    double* partition = malloc((partition_size+1)*sizeof(double));

    int noperations = 1;
    enum operations {READING};
    char* operation_names[1] = {"reading"};

    prb_stopwatch_t* sw = prb_stopwatch_new(noperations);
    prb_stopwatch_start(sw, READING);

    // Open file
    MPI_File f;
    MPI_File_open(MPI_COMM_WORLD, "data.bin", MPI_MODE_RDONLY, MPI_INFO_NULL, &f);

    // Otherwise MPI_Barrier has no effect.
    if (debug_mode) MPI_File_set_atomicity(f, 1);

    // Seek to partition first index.
    MPI_File_set_view(f, i0*sizeof(double), MPI_DOUBLE, MPI_DOUBLE,
                      "native", MPI_INFO_NULL);

    // Read partition from file.
    MPI_Status status;
    MPI_File_read(f, partition, partition_size, MPI_DOUBLE, &status);

    if (debug_mode) check_data_mean(partition, partition_size, array_size);

    // Close file.
    MPI_File_close(&f);

    prb_stopwatch_stop(sw, READING);
    prb_stopwatch_write_csv(sw, output_filepath,
                            noperations, operation_names,
                            MPI_COMM_WORLD, 0);
    
    prb_stopwatch_free(sw);

    // Finalize MPI.
    MPI_Finalize();

    free(partition);

    return 0;
}
