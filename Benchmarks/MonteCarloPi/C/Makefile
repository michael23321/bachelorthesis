CC = gcc
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra 

TARGETS = SequentialRand_r ParallelRand_r SequentialXoshiro ParallelXoshiro
SRCS    = SequentialRand_r.c ParallelRand_r.c SequentialXoshiro.c ParallelXoshiro.c 

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)
