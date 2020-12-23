#include <stdlib.h>
#include <string.h> // memcpy

#include "mem.h"
#include "graph/graph.h"
#include "graph/serial.h"

/* internal macros for gr_serialize_linear */
#define _gr_push_atom_type(buffer, buffer_size, used_size, atomtype) { \
        ensure_space(&buffer, &buffer_size, used_size, sizeof(atom_type)); \
        memcpy(buffer + used_size, atomtype, sizeof(atom_type)); \
        used_size += sizeof(atom_type); \
    }
#define _gr_push_atom(buffer, buffer_size, used_size, atom, atomtype, current_id) { \
        ensure_space(&buffer, &buffer_size, used_size, sizeof(atomtype)); \
        memcpy(buffer + used_size, &atom, sizeof(atomtype)); \
        used_size += sizeof(atomtype); \
        current_id += 1; \
    }

char *gr_serialize_linear(gr_node **nodes, uint32_t count, uint32_t *size) {
    char *buffer;
    uint32_t buffer_size = MEM_ALLOC_BLOCK, used_size = 0;
    atomid current_id;
    atom_type atype;

    alloc_null(buffer, buffer_size);

    // write all nodes
    for(int i = 0; i < count; ++i) {
        gr_node *current_node = *(nodes + sizeof(void*) * i);
        struct gr_node_atom current_node_atom;

        current_node_atom.id = current_node->id;
        current_node_atom.addr = current_node->addr;

        current_node_atom.neighbor_count = current_node->neighbor_count;
        if(current_node->neighbor_count > 0) {
            current_node_atom.neighbors = current_id + 1; // neighbor list begins with next atom
        } else {
            current_node_atom.neighbors = ATOMID_INVALID;
        }

        atype = ATYPE_NODE;
        _gr_push_atom_type(buffer, buffer_size, used_size, &atype);
        _gr_push_atom(buffer, buffer_size, used_size, current_node_atom, struct gr_node_atom, current_id);

        /* write neighbors */
        for(int ni = 0; ni < current_node->neighbor_count; ++ni) {
            struct gr_list_atom current_neighbor_atom;

            current_neighbor_atom.current = (*(current_node->neighbor + sizeof(void*) * ni))->id;
            current_neighbor_atom.tail = current_id + 1;

            atype = ATYPE_LIST;
            _gr_push_atom_type(buffer, buffer_size, used_size, &atype);
            _gr_push_atom(buffer, buffer_size, used_size, current_neighbor_atom, struct gr_list_atom, current_id);
        }
    }

    realloc_or_fail(&buffer, used_size);
    *size = used_size;
    return buffer;
}

char *gr_serialize(gr_node *graph, uint32_t *size) {
    gr_node **nodes;
    uint32_t count = 0;

    nodes = gr_linearize(graph, &count);

    return gr_serialize_linear(nodes, count, size);
}
