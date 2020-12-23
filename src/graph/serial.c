#include <stdlib.h>

#include "mem.h"
#include "graph/graph.h"
#include "graph/serial.h"

/* internal macros for gr_serialize_linear */
#define _gr_push_atom_type(buffer, buffer_size, used_size, atomtype) { \
        ensure_space(&buffer, &buffer_size, used_size, sizeof(atom_type)); \
        memcpy(buffer + used_space, &atom_type, sizeof(atom_type)); \
        used_size += sizeof(atom_type); \
    }
#define _gr_spush_atom(buffer, buffer_size, used_size, atom, atomtype, current_id) { \
        ensure_space(&buffer, &buffer_size, used_size, sizeof(atomtype)); \
        memcpy(buffer + used_space, &atom, sizeof(atomtype)); \
        used_size += sizeof(atomtype); \
        current_id += 1; \
    }

char *gr_serialize_linear(gr_node **nodes, uint32_t count, uint32_t *size) {
    char *buffer;
    uint32_t buffer_size = MEM_ALLOC_BLOCK, used_size = 0;
    atomid current_id;

    alloc_null(buffer, buffer_size);

    // write all nodes
    for(int i = 0; i < count; ++i) {
        gr_node *current_node = *(nodes + sizeof(void*) * i);
        struct gr_node_atom current_node_atom;

        current_node_atom.id = current->id;
        current_node_atom.addr = current->addr;

        current_node_atom.neighbor_count = current_node->neighbor_count;
        if(current_node->neighbor_count > 0) {
            current_node_atom.neighbors = current_id + 1; // neighbor list begins with next atom
        } else {
            current_node_atom.neighbors = ATOMID_INVALID;
        }

        _gr_push_atom_type(buffer, buffer_size, used_space, ATYPE_NODE);
        _gr_push_atom(buffer, buffer_size, used_space, current_node_atom, struct gr_node_atom, current_id);

        /* write neighbors */
        for(int ni = 0; ni < current_node->neighbor_count; ++ni) {
            struct gr_list_atom current_neighbor_atom;

            current_neighbor_atom.current = *(current_node->neighbors + sizeof(void*) * ni)->id;
            current_neighbor_atom.tail = current_id + 1;

            _gr_push_atom_type(buffer, buffer_size, used_space, ATYPE_LIST);
            _gr_push_atom(buffer, buffer_size, used_space, current_neighbor_atom, struct gr_list_atom, current_id);
        }
    }

    realloc_or_fail(&buffer, used_space);
    *size = used_space;
    return buffer;
}

char *gr_serialize(gr_node *graph, uint32_t *size) {
    gr_node **nodes;
    uint32_t count = 0;

    nodes = gr_linearize(graph, &count);

    return gr_serialize_linear(nodes, count, size);
}
