#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ITERATION_COUNT 10000


typedef struct {
    struct timeval start, end;
}benchmark;

void bm_start(benchmark *bm) {
    gettimeofday(&bm->start, NULL);
}

void bm_end(benchmark *bm) {
    gettimeofday(&bm->end, NULL);
}

void bm_write(benchmark *bm, char *name) {
    printf("%s\t%ld\n",
            name,
            (1000000 * (bm->end.tv_sec - bm->start.tv_sec) + bm->end.tv_usec - bm->start.tv_usec));
}

#endif
