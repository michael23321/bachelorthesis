CC = gcc
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra 
LDFLAGS = -lm

TARGETS = Sequential ParallelStatic ParallelDynamic
SRCS    = Sequential.c ParallelStatic.c ParallelDynamic.c  

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) mandelbrot.png
