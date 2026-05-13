#pragma once

#include <gsl/gsl_rng.h>

/**
 * Выполняет локальную оценку π методом Монте-Карло
 * @param rng Инициализированный генератор GSL
 * @param points_count Количество точек для обработки
 * @return Количество точек, попавших в четверть круга
 */
long long monte_carlo_step(gsl_rng* rng, long long points_count);

/**
 * Создаёт и настраивает генератор случайных чисел GSL
 * @param seed Уникальное зерно для процесса
 * @return Указатель на инициализированный генератор
 */
gsl_rng* create_rng(unsigned int seed);