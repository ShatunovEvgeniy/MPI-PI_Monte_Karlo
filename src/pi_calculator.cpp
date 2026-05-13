#include "pi_calculator.hpp"
#include <cmath>

gsl_rng* create_rng(unsigned int seed) {
    const gsl_rng_type* T = gsl_rng_mt19937;
    gsl_rng* rng = gsl_rng_alloc(T);
    gsl_rng_set(rng, seed);
    return rng;
}

long long monte_carlo_step(gsl_rng* rng, long long points_count) {
    long long hits = 0;
    
    for (long long i = 0; i < points_count; ++i) {
        double x = gsl_rng_uniform(rng);
        double y = gsl_rng_uniform(rng);
        
        if (x * x + y * y <= 1.0) {
            ++hits;
        }
    }
    
    return hits;
}