#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

double
compute_data_mean_expected(size_t problem_size);

void
check_data_mean(double* partiton, size_t partition_size, size_t problem_size);

#endif
