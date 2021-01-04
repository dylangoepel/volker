#ifndef SERIAL_H_
#define SERIAL_H_

#include "graph/graph.h"

#define ATOMID_INVALID 0
typedef uint32_t atomid;

#define ATYPE_EOF  0
#define ATYPE_NODE 1
#define ATYPE_LIST 2
typedef uint8_t atom_type;

struct gr_node_atom {
    // current node
    vlkr_id id;
    uint32_t addr;

    // neighbors
    atomid neighbors;
};

struct gr_list_atom {
    // current node
    vlkr_id current;

    // tail
    atomid tail;
};

char *gr_serialize(gr_node *graph, uint32_t *size);
gr_node **gr_deserialize(char *buffer, uint32_t size, int *node_count);

#endif
