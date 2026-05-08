#include <iostream>
#include <mpi.h>
#include <gsl/gsl_rng.h>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_POINTS = 100'000'000LL;
    long long points_per_proc = TOTAL_POINTS / size;
    long long remainder = TOTAL_POINTS % size;
    if (rank < remainder) points_per_proc++; // Равномерное распределение с учётом остатка

    const gsl_rng_type* T = gsl_rng_mt19937;
    gsl_rng* r = gsl_rng_alloc(T);
    gsl_rng_set(r, rank + 42);

    long long local_hits = 0;
    
    // Старт
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    for (long long i = 0; i < points_per_proc; ++i) {
        double x = gsl_rng_uniform(r);
        double y = gsl_rng_uniform(r);
        if (x * x + y * y <= 1.0) {
            local_hits++;
        }
    }

    double t_end = MPI_Wtime();
    double local_elapsed = t_end - t_start;

    // Сбор результатов
    long long total_hits = 0;
    double max_elapsed = 0.0;

    MPI_Reduce(&local_hits, &total_hits, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_est = 4.0 * total_hits / TOTAL_POINTS;
        std::cout << "=== Оценка числа Pi (Monte Carlo) ===\n";
        std::cout << "Процессов MPI:  " << size << "\n";
        std::cout << "Всего точек:    " << TOTAL_POINTS << "\n";
        std::cout << "Оценка Pi:      " << pi_est << "\n";
        std::cout << "Время (с):      " << max_elapsed << "\n";
        std::cout << "====================================\n";
    }

    gsl_rng_free(r);
    MPI_Finalize();
    return 0;
}