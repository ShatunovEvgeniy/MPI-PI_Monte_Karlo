#include "mpi_helpers.hpp"

long long distribute_points(long long total, int rank, int size) {
    long long base = total / size;
    long long remainder = total % size;
    return base + (rank < remainder ? 1 : 0);
}

void reduce_result(const long long& local_value, long long* global_value, MPI_Op op) {
    MPI_Reduce(&local_value, global_value, 1, MPI_LONG_LONG, op, 0, MPI_COMM_WORLD);
}

void reduce_result(const double& local_value, double* global_value, MPI_Op op) {
    MPI_Reduce(&local_value, global_value, 1, MPI_DOUBLE, op, 0, MPI_COMM_WORLD);
}