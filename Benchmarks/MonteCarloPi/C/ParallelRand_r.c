#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <omp.h>

#define ITERATIONS 700000000
#define START_SEED 0

double monte_carlo_pi(long iterations) {
    long points_in_circle = 0;

    #pragma omp parallel reduction(+:points_in_circle)
    {
        unsigned int seed = START_SEED + omp_get_thread_num();

        #pragma omp for
        for (long i = 0; i < iterations; i++) {
            double x = rand_r(&seed) / (double) RAND_MAX;
            double y = rand_r(&seed) / (double) RAND_MAX;

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