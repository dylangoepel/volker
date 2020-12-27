#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "benchmark.h"
#include "graph/graph.h"

int main() {
    gr_node *nodes[ITERATION_COUNT];
    benchmark b;

    bm_start(&b);
    for(int i = 0; i < ITERATION_COUNT; ++i)
        nodes[i] = gr_new(i, i);
    bm_end(&b);
    bm_write(&b, "gr_new");
    for(int i = 0; i < ITERATION_COUNT; ++i)
        free(nodes[i]);

    return 0;
}
