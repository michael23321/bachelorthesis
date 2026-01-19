#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>
#include <stdint.h>

#define ITERATIONS 700000000

static inline double to_double(uint64_t x) {
    uint64_t top53 = x >> 11;
    const double scale = 1.0 / ( (uint64_t)1 << 53 );
    return top53 * scale;
}

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

typedef struct {
    uint64_t s[4];
} xoshiro256_state;

uint64_t next(xoshiro256_state* state) {
	const uint64_t result = rotl(state->s[0] + state->s[3], 23) + state->s[0];
    const uint64_t t = state->s[1] << 17;
    state->s[2] ^= state->s[0];
    state->s[3] ^= state->s[1];
    state->s[1] ^= state->s[2];
    state->s[0] ^= state->s[3];
    state->s[2] ^= t;
    state->s[3] = rotl(state->s[3], 45);
    return result;
}

uint64_t splitmix64_next(uint64_t* state) {
    uint64_t z = (*state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

void seed_xoshiro(xoshiro256_state* rng_state, uint64_t seed) {
    uint64_t sm_state = seed;
    for (int i = 0; i < 4; i++) {
        rng_state->s[i] = splitmix64_next(&sm_state);
    }
}

double monte_carlo_pi(long iterations) {
    long points_in_circle = 0;

    #pragma omp parallel reduction(+:points_in_circle)
    {
        xoshiro256_state state;
        int thread_id = omp_get_thread_num();
        seed_xoshiro(&state, 1234 + thread_id * 9999);

        #pragma omp for
        for (long i = 0; i < iterations; i++) {
            double x = to_double(next(&state));
            double y = to_double(next(&state));

            if (x * x + y * y <= 1.0f) {
                points_in_circle++;
            }
        }
    }

    return 4.0 * (points_in_circle / (double) iterations);
}



int main() {
    int n_runs = 20;
    double *times = malloc(n_runs * sizeof(double));

    for (int run = 0; run < n_runs; run++) {
        double start = omp_get_wtime();
        double pi = monte_carlo_pi(ITERATIONS);
        double end = omp_get_wtime();
        times[run] = end - start;
        printf("Pi: %f\n", pi);
    }

    printf("--- Times ---\n");
    double sum = 0.0;
    printf("[");

    for (int run = 0; run < n_runs; run++) {
        if (run > 0) {
            printf(", ");
        }
        printf("%f", times[run]);
        sum += times[run];
    }

    printf("]\n");

    printf("\n--- Statistics ---\n");
    printf("Average: %f\n", sum / n_runs);

    free(times);
    return EXIT_SUCCESS;
}