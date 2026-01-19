#include <stdio.h>
#include <stdlib.h>
#include <omp.h>


long delannoy(int m, int n) {
    if (m == 0 || n == 0) {
        return 1;
    } else {
        return delannoy(m - 1, n) + delannoy(m - 1, n - 1) + delannoy(m, n - 1);
    }
}


int main(int argc, char * argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: [n]\n");
		return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    int n_runs = 20;
    double *times = malloc(n_runs * sizeof(double));

    for (int run = 0; run < n_runs; run++) {
        double start = omp_get_wtime();
        long result = delannoy(n, n);
        double end = omp_get_wtime();
        times[run] = end - start;
        printf("%ld\n", result);
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


    return EXIT_SUCCESS;
}