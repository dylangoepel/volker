#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "graph/graph.h"

#define BENCHMARK_COUNT 10
#define ITER_COUNT 100000

int main() {
    gr_node *nodes[ITER_COUNT];
    struct timeval start, end;

    for(int b = 0; b < BENCHMARK_COUNT; ++b) {
        gettimeofday(&start, NULL);
        for(int i = 0; i < ITER_COUNT; ++i)
            nodes[i] = gr_new(i, i);
        gettimeofday(&end, NULL);
        printf("%d\t%ld\n",
                b,
                (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec));
        for(int i = 0; i < ITER_COUNT; ++i)
            free(nodes[i]);
    }

    return 0;
}
