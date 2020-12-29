#include <stdlib.h>
#include <string.h> // memcpy

#include "mem.h"
#include "graph/graph.h"
#include "graph/serial.h"

static inline void *__push_onto_buffer(char **buffer, uint32_t *buffer_size, uint32_t *used_size, char *data, uint32_t data_size) {
    ensure_space(buffer, buffer_size, *used_size, data_size);
    memcpy(*buffer + *used_size, data, data_size);
    *used_size += data_size;
    return buffer + *used_size;
}

char *gr_serialize_linear(gr_node **nodes, uint32_t count, uint32_t *size) {
    char *buffer;
    uint32_t buffer_size = MEM_ALLOC_BLOCK, used_size = 0;
    atomid current_id = 0;
    atom_type atype;

    alloc_null(buffer, buffer_size);

    // write all nodes
    for(int i = 0; i < count; ++i) {
        gr_node *current_node = *(nodes + i);
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
        __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&atype), sizeof(atype));
        __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&current_node_atom), sizeof(current_node_atom));

        /* write neighbors */
        for(int ni = 0; ni < current_node->neighbor_count; ++ni) {
            struct gr_list_atom current_neighbor_atom;

            current_neighbor_atom.current = (*(current_node->neighbor + sizeof(void*) * ni))->id;
            current_neighbor_atom.tail = current_id + 1;

            atype = ATYPE_LIST;
            __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&atype), sizeof(atype));
            __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&current_neighbor_atom), sizeof(current_neighbor_atom));
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
