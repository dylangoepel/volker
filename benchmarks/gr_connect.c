#include <stdio.h>
#include <stdlib.h>
#include "benchmark.h"
#include "graph/graph.h"

#define NODE_COUNT 1000

int main() {
    benchmark b;
    gr_node *nodes[NODE_COUNT];

    for(int i = 0; i < ITERATION_COUNT; ++i) {
        // create nodes
        for(int i = 0; i < NODE_COUNT; ++i) {
            nodes[i] = gr_new(i, 0);
        }

        // benchmark single connections
        bm_start(&b);
        for(int i = 0; i < NODE_COUNT; ++i) {
            gr_connect(nodes[i], nodes[(i + 1) % NODE_COUNT]);
        }
        bm_end(&b);
        bm_write(&b, "gr_connect single");

        // benchmark double connections
        bm_start(&b);
        for(int i = 0; i < NODE_COUNT; ++i) {
            gr_connect(nodes[i], nodes[(i + 2) % NODE_COUNT]);
        }
        bm_end(&b);
        bm_write(&b, "gr_connect double");

        // benchmark double connections
        bm_start(&b);
        for(int i = 0; i < NODE_COUNT; ++i) {
            gr_connect(nodes[i], nodes[(i + 3) % NODE_COUNT]);
        }
        bm_end(&b);
        bm_write(&b, "gr_connect triple");
    }
}
