#include "graph/graph.h"

int main() {
    gr_node *nodes[6];

    // initialize nodes
    for(int i = 0; i < 6; ++i) {
        nodes[i] = gr_new(i, 0);
    }

    // build up graph
    for(int i = 0; i < 6; ++i) {
        gr_connect(nodes[i > 0 ? i - 1 : 4], nodes[i]);
    }

    gr_node *result = gr_find_by_id(nodes[0], 5);
    if(result->id == 5)
        return 0;
    return 1;
}
