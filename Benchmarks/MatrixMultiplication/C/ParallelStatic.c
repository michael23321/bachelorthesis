#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define R1 1000
#define C1 1000
#define R2 1000
#define C2 1000


void matrix_multiplication(double *A, double *B_t, double *C) {   
    #pragma omp parallel for 
    for (int i = 0; i < R1; i++) {
        for (int j = 0; j < C2; j++) {
            double s = 0.0;
            for (int k = 0; k < R2; k++) {
                s += A[i * C1 + k] * B_t[j * R2 + k]; 
            }
            C[i * C2 + j] = s;
        }
    }
}

double* create_matrix(int rows, int cols) {
    double *mat = malloc(rows * cols * sizeof(double));
    return mat;
}

void free_matrix(double *mat) {
    free(mat);
}

void fill_random(double *mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i * cols + j] = (double)rand() / RAND_MAX;
        }
    }
}

void transpose_matrix(double *src, double *dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[j * rows + i] = src[i * cols + j];
        }
    }
}

int main() {
    double *A = create_matrix(R1, C1);
    double *B = create_matrix(R2, C2);
    double *C = create_matrix(R1, C2);

    double *B_T = create_matrix(C2, R2);

    srand(1234);
    fill_random(A, R1, C1);
    fill_random(B, R2, C2);

    transpose_matrix(B, B_T, R2, C2);

    int n_runs = 50;
    double *times = malloc(n_runs * sizeof(double));

    for (int run = 0; run < n_runs; run++) {
        double start = omp_get_wtime();
        matrix_multiplication(A, B_T, C);
        double end = omp_get_wtime();
        times[run] = end - start;
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

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return EXIT_SUCCESS;
}