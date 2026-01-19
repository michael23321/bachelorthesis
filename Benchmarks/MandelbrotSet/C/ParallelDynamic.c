#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define X 1920
#define Y 1080
#define MAX_ITER 10000

void calc_mandelbrot(uint8_t image[Y][X]) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < Y; i++) {
        double cy = (double)i / Y * 2 - 1;
        for (int j = 0; j < X; j++) {
            double x = 0.0;
            double y = 0.0;
            double cx = (double)j / X * 3.5 - 2.5;
            double iteration = 0;
            while (x*x + y*y <= 4.0 && iteration < MAX_ITER) {
                double x_tmp = x*x -  y*y + cx;
                y = 2*x*y + cy;
                x = x_tmp;
                iteration = iteration + 1;
            }
            double norm_iteration = (iteration / MAX_ITER) * 255;
            image[i][j] = norm_iteration;
        }
    }
}

int main() {
	uint8_t image[Y][X];

    int n_runs = 20;
    double *times = malloc(n_runs * sizeof(double));

    for (int run = 0; run < n_runs; run++) {
        double start = omp_get_wtime();
        calc_mandelbrot(image);
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

	const int channel_nr = 1, stride_bytes = 0;
	stbi_write_png("mandelbrot.png", X, Y, channel_nr, image, stride_bytes);
	return EXIT_SUCCESS;
}