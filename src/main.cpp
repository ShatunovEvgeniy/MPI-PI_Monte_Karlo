#include <iostream>
#include <mpi.h>

#include "config.hpp"
#include "pi_calculator.hpp"
#include "mpi_helpers.hpp"

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Распределяем нагрузку
    long long local_points = distribute_points(
        config::TOTAL_POINTS, rank, size
    );

    // Инициализируем ГПСЧ с уникальным seed
    auto rng = create_rng(rank + config::RNG_SEED_OFFSET);

    // Синхронизация перед замером времени
    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    // Основной расчёт
    long long local_hits = monte_carlo_step(rng, local_points);

    double t_end = MPI_Wtime();
    double local_elapsed = t_end - t_start;

    // Сбор результатов
    long long total_hits = 0;
    double max_elapsed = 0.0;

    reduce_result(local_hits, &total_hits, MPI_SUM);
    reduce_result(local_elapsed, &max_elapsed, MPI_MAX);

    // Вывод результата
    if (rank == 0) {
        double pi_est = 4.0 * total_hits / config::TOTAL_POINTS;
        
        std::cout << "=== Оценка числа Pi (Monte Carlo) ===\n";
        std::cout << "Процессов MPI:  " << size << "\n";
        std::cout << "Всего точек:    " << config::TOTAL_POINTS << "\n";
        std::cout << "Оценка Pi:      " << pi_est << "\n";
        std::cout << "Время (с):      " << max_elapsed << "\n";
        
        if (size > 1) {
            double speedup = (max_elapsed > 0) ? 
                (local_elapsed * size) / max_elapsed : 0;
            std::cout << "Ускорение:      ~" << speedup << "x\n";
        }
        std::cout << "====================================\n";
    }

    // Очистка
    gsl_rng_free(rng);
    MPI_Finalize();
    return 0;
}