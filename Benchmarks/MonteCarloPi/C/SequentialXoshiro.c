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

static uint64_t s[4];

uint64_t next(void) {
	const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result;
}

static uint64_t splitmix64_state;

uint64_t splitmix64_next(void) {
    uint64_t z = (splitmix64_state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}

void seed_xoshiro(uint64_t seed) {
    splitmix64_state = seed;
    for (int i = 0; i < 4; i++) {
        s[i] = splitmix64_next();
    }
}

double monte_carlo_pi(long iterations) {
    long points_in_circle = 0;
    for (long i = 0; i < iterations; i++) {
        double x = to_double(next());
        double y = to_double(next());

        if (x * x + y * y <= 1.0f) {
            points_in_circle++;
        }
    }

    return 4.0 * (points_in_circle / (double) iterations);
}



int main() {
    int n_runs = 20;
    double *times = malloc(n_runs * sizeof(double));

    uint64_t seed = 1234;
    seed_xoshiro(seed);

    for (int run = 0; run < n_runs; run++) {
        uint64_t seed = 1234;
        seed_xoshiro(seed);
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