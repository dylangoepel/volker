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
            current_neighbor_atom.tail = ni == current_node->neighbor_count - 1 ? ATOMID_INVALID : current_id + 1;

            atype = ATYPE_LIST;
            __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&atype), sizeof(atype));
            __push_onto_buffer(&buffer, &buffer_size, &used_size, ((char *)&current_neighbor_atom), sizeof(current_neighbor_atom));
        }
    }

    realloc_or_fail(buffer, used_size);
    *size = used_size;
    return buffer;
}

char *gr_serialize(gr_node *graph, uint32_t *size) {
    gr_node **nodes;
    uint32_t count = 0;
    char *ret;

    nodes = gr_linearize(graph, &count);

    ret = gr_serialize_linear(nodes, count, size);
    free(nodes);
    return ret;
}

static inline int _gr_atype_to_size(atom_type t) {
    switch(t) {
        case ATYPE_NODE:
            return sizeof(struct gr_node_atom);
        case ATYPE_LIST:
            return sizeof(struct gr_list_atom);
        default:
            return 0;
    }
}

void *_gr_ser_cdr(char *buffer) {
    atom_type type;
    int offset;

    memcpy(&type, buffer, sizeof(atom_type));
    
    if((offset = _gr_atype_to_size(type)) == 0)
        return NULL;

    return buffer + sizeof(atom_type) + offset;
}

// read the nth atom of buffer or return NULL in case in case of error
void *gr_seek_atom(char *buffer, uint32_t size, atomid id, atom_type *type) {
    const char *bound = buffer + size;
    atomid cid = 0;
    while((buffer < bound) && (cid < id)) {
        buffer = _gr_ser_cdr(buffer);
        ++cid;
    }

    if(bound - buffer <= sizeof(atom_type)) // if the loop reached the buffer's end without finding the atom in question
        return NULL;

    memcpy(type, buffer, sizeof(atom_type));

    if(bound - buffer <= sizeof(atom_type) + _gr_atype_to_size(*type))
        return NULL;

    return buffer + sizeof(atom_type);
}

vlkr_id *gr_deserialize_list(char *buffer, uint32_t buffer_size, atomid location, int *count) {
    atom_type t;
    struct gr_list_atom *item;
    int allocated_size;

    allocated_size = MEM_ALLOC_BLOCK;

    vlkr_id *ret;
    alloc_null(ret, allocated_size);

    // TODO prevent infinite recursion

    for(int i = 0; 1; ++i) {
        if(location == ATOMID_INVALID) {
            *count = i;
            (void) realloc(ret, i * sizeof(vlkr_id));
            return ret;
        }

        item = (struct gr_list_atom *) gr_seek_atom(buffer, buffer_size, location, &t);
        if((t != ATYPE_LIST) || (item == NULL)) {
            free(ret);
            *count = 0;
            return NULL;
        }

        ensure_space(ret, &allocated_size, i * sizeof(vlkr_id), sizeof(vlkr_id));
        ret[i] = item->current;

        location = item->tail;
    }
}

gr_node **gr_deserialize(char *buffer, uint32_t size, int *node_count) {
    atom_type t;
    struct gr_node_atom *atom;
    vlkr_id *ids;
    int ids_size;

    const char *bound = buffer + size;
    char *location = buffer;

    gr_node **nodes;
    uint32_t i;
    int allocated_size = MEM_ALLOC_BLOCK;

    alloc_null(nodes, allocated_size);

    // collect all nodes
    for(i = 0;  bound - location >= sizeof(atom_type); ++i) {
        memcpy(&t, location, sizeof(atom_type));
        if(t == ATYPE_EOF)
            break;
        if(t == ATYPE_LIST) {
            location = _gr_ser_cdr(location); // step to next atom
            --i;
            continue;
        }

        atom = (struct gr_node_atom *) (location + sizeof(atom_type));
        ids = gr_deserialize_list(buffer, size, atom->neighbors, &ids_size);
        if(ids == NULL) {
            // TODO free previous id arrays
            return NULL;
        }

        nodes[i] = gr_new(atom->id, atom->addr);

        // store list of vlkr_ids, which will be resolved in a second step
        nodes[i]->neighbor = (struct gr_node **) ids;
        nodes[i]->neighbor_count = ids_size;

        location = _gr_ser_cdr(location);
    }

    (void) realloc(nodes, i * sizeof(struct gr_node));

    // resolve neighbors of nodes 
    for(int k = 0; k < i; ++k) {
        ids = (vlkr_id *) nodes[k]->neighbor;

        nodes[k]->neighbor = malloc(nodes[k]->neighbor_count * sizeof(struct gr_node *));
        for(int n = 0; n < nodes[k]->neighbor_count; ++n) {
            if((nodes[k]->neighbor[n] = gr_find_by_id_linear(nodes, i, ids[n])) == NULL) {
                free(nodes);
                // TODO free everything
                return NULL;
            }
        }
        free(ids);
    }

    *node_count = i;
    return nodes;
}
