#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "benchmark.h"
#include "graph/graph.h"

#define NODE_COUNT 1000

int main() {
    gr_node *nodes[NODE_COUNT];
    benchmark b;

    for(int i = 0; i < ITERATION_COUNT; ++i) {
        bm_start(&b);
        for(int i = 0; i < NODE_COUNT; ++i)
            nodes[i] = gr_new(i, i);
        bm_end(&b);
        bm_write(&b, "gr_new");
        for(int i = 0; i < NODE_COUNT; ++i)
            free(nodes[i]);
    }

    return 0;
}
