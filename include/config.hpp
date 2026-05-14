#pragma once

// Общие константы проекта
namespace config {
    constexpr long long TOTAL_POINTS = 15000000000LL;
    constexpr unsigned int RNG_SEED_OFFSET = 42;
    
    // Вывод отладочной информации
    #ifdef DEBUG
        constexpr bool VERBOSE = true;
    #else
        constexpr bool VERBOSE = false;
    #endif
}