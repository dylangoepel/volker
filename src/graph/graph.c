#include "graph/graph.h"

#include <stdlib.h>

// exhaustive search for pointer in pointer array, returns bool
uint8_t __chk_ptr_in_array(void **a, uint32_t c, void *ptr) {
    for(uint32_t i = 0; i < c; ++i) {
        if(a[i * sizeof(void*)] == ptr)
            return 1;
    }
    return 0;
}
static inline int __arr_append_ptr(void **arr[], uint32_t *count, uint32_t *size, void *ptr) {
    if(*size < (*count + 1) * sizeof(void *)) {
        void *newarr = realloc(*arr, *size + GR_LINEARIZE_BUFSIZE);
        if(newarr == NULL) {
            return  -1;
        }

        *arr = newarr;
        *size += GR_LINEARIZE_BUFSIZE;
    }

    *(*arr + *count * sizeof(void*)) = ptr;
    (*count)++;

    return 0;
}

gr_node *gr_new(vlkr_id id, uint32_t addr) {
    gr_node *node = malloc(sizeof(gr_node));

    if(node == NULL) { // allocation error
        return NULL;
    }

    node->id = id;
    node->addr = addr;

    node->neighbor = NULL;
    node->neighbor_count = 0;

    return node;
}

int gr_connect(gr_node *n1, gr_node *n2) {
    gr_node **newarray;

    if(n1->neighbor_count == 0)
        newarray = malloc(sizeof(gr_node*));
    else
        newarray = realloc(n1->neighbor, sizeof(gr_node*) * (n1->neighbor_count + 1));

    if(newarray == NULL) // on allocation error
        return -1;

    newarray[n1->neighbor_count] = n2;
    ++(n1->neighbor_count);

    n1->neighbor = newarray;

    return 0;
}

gr_node **gr_linearize(gr_node *graph, uint32_t *count) {
    uint32_t current_bufsize = GR_LINEARIZE_BUFSIZE * sizeof(gr_node), node_count = 0, node_id = 0;
    gr_node **buffer, *current = graph;

    buffer = malloc(current_bufsize);
    if(buffer == NULL) {
        return NULL;
    }

    uint8_t dead_end = 0;
    while(1) {
        if(!dead_end)
            __arr_append_ptr((void ***) &buffer, &node_count, &current_bufsize, current);

        dead_end = 1;
        for(uint32_t i = 0; i < current->neighbor_count; ++i) {
            if(!__chk_ptr_in_array((void **) buffer, node_count, current->neighbor[i])) {
                current = current->neighbor[i];
                dead_end = 0;
                break;
            }
        }

        if(dead_end && (node_id == 0)) {
            break;
        } else if(dead_end) {
            current = buffer[(node_id - 1) * sizeof(void*)];
            node_id--;
        } else {
            node_id = node_count;
        }
    }

    buffer = realloc(buffer, node_count * sizeof(void*));
    *count = node_count;
    return buffer;
}

gr_node *gr_find_by_id(gr_node *graph, vlkr_id id) {
    uint32_t node_count;
    gr_node **nodes = gr_linearize(graph, &node_count);

    for(uint32_t i = 0; i < node_count; ++i) {
        if(nodes[i * sizeof(void*)]->id == id) {
            gr_node *ret = nodes[i * sizeof(void*)];
            free(nodes);
            return ret;
        }
    }

    return NULL;
}
