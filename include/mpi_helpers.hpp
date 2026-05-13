#pragma once

#include <mpi.h>

/**
 * Равномерно распределяет общее количество точек между процессами
 * @param total Общее число точек
 * @param rank Ранг текущего процесса
 * @param size Общее количество процессов
 * @return Количество точек для текущего процесса
 */
long long distribute_points(long long total, int rank, int size);

/**
 * Собирает результаты со всех процессов на ранге 0
 * @param local_value Локальное значение
 * @param global_value Указатель для результата (только на ранге 0)
 * @param op Операция редукции (MPI_SUM, MPI_MAX и т.д.)
 */
void reduce_result(const long long& local_value, long long* global_value, MPI_Op op);
void reduce_result(const double& local_value, double* global_value, MPI_Op op);