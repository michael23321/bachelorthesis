CC = gcc
CFLAGS = -O3 -march=native -fopenmp -Wall -Wextra 

TARGETS = Sequential ParallelStatic
SRCS    = Sequential.c ParallelStatic.c

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)
