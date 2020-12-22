#ifndef GRAPH_H_
#define GRAPH_H_

#include <stdint.h>

#ifndef GR_LINEARIZE_BUFSIZE
#define GR_LINEARIZE_BUFSIZE 1000 // increase the node buffer by 100 * sizeof(gr_node) steps in gr_linearize
#endif

#define VLKR_ID_INALID 0
#define VLKR_ID_ROOT 1

typedef uint16_t vlkr_id;

struct gr_node {
    // current node
    vlkr_id id;
    uint32_t addr;

    // neighbors
    struct gr_node **neighbor;
    uint32_t neighbor_count;
};

typedef struct gr_node gr_node;

// basic graph manipulation
gr_node *gr_new(vlkr_id id, uint32_t addr);
int gr_connect(gr_node *n1, gr_node *n2);

gr_node **gr_linearize(gr_node *graph, uint32_t *count);

// recursive searching
gr_node *gr_find_by_id(gr_node *graph, vlkr_id id);

#endif
