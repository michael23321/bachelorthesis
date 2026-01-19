#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

long delannoy_seq(int m, int n) {
    if (m == 0 || n == 0) {
        return 1;
    } else {
        return delannoy_seq(m - 1, n) + delannoy_seq(m - 1, n - 1) + delannoy_seq(m, n - 1);
    }
}

long delannoy_par(int m, int n) {
    long result1, result2, result3;

    if (m == 0 || n == 0) {
        return 1;
    } else if (m < 9 || n < 9) {
        return delannoy_seq(m, n);
    }

    #pragma omp task shared(result1) 
    {
        result1 = delannoy_par(m - 1, n);
    }
    #pragma omp task shared(result2) 
    {
        result2 = delannoy_par(m - 1, n - 1);
    }

    #pragma omp task shared(result3)
    {
        result3 = delannoy_par(m, n - 1);
    }
    

    #pragma omp taskwait
    return result1 + result2 + result3;
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

        long result;
        #pragma omp parallel
        {
            #pragma omp single
            {
                result = delannoy_par(n, n);
            }
        }
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

    free(times);

    return EXIT_SUCCESS;
}