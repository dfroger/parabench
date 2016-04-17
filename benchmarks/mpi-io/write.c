#include <stdio.h>
#include <stdlib.h>

#include "tool.h"

#include "mpi.h"
#include "parabench.h"

int
main(int argc, char **argv)
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
    double* partition = (double*) malloc(partition_size*sizeof(double));

    // Set data partition values.
    size_t i;
    for (i = 0 ; i < partition_size ; i++)
        partition[i] = i0 + i;

    int noperations = 1;
    enum operations {WRITING};
    char* operation_names[1] = {"writting"};

    prb_stopwatch_t* sw = prb_stopwatch_new(noperations);
    prb_stopwatch_start(sw, WRITING);

    // Open file
    MPI_File f;
    int mode = MPI_MODE_CREATE | MPI_MODE_WRONLY;
    char filename[] = "data.bin";
    MPI_File_open(MPI_COMM_WORLD, filename, mode, MPI_INFO_NULL, &f);

    // Seek to partition first index.
    MPI_File_set_view(f, i0*sizeof(double), MPI_DOUBLE, MPI_DOUBLE,
                      "native", MPI_INFO_NULL);

    // Write partition to file.
    MPI_Status status;
    MPI_File_write(f, partition, partition_size, MPI_DOUBLE, &status);

    // Close file.
    MPI_File_close(&f);

    prb_stopwatch_stop(sw, WRITING);
    prb_stopwatch_write_csv(sw, output_filepath,
                            noperations, operation_names,
                            MPI_COMM_WORLD, 0);
    
    prb_stopwatch_free(sw);

    // Finalize MPI.
    MPI_Finalize();

    return 0;
}
