CC = gcc
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra 

TARGETS = Sequential ParallelWithCutoff ParallelWithoutCutoff
SRCS    = Sequential.c ParallelWithCutoff.c ParallelWithoutCutoff.c 

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)
