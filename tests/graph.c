#include "graph/graph.h"
#include "graph/serial.h"

#include <stdlib.h>

#define EXIT_FIND_ERROR 1
#define EXIT_SERIALIZE_ERROR 2

int main() {
    gr_node *nodes[6];

    /* build graph */

    for(int i = 0; i < 6; ++i) {
        nodes[i] = gr_new(i, 0);
    }

    for(int i = 0; i < 6; ++i) {
        gr_connect(nodes[i > 0 ? i - 1 : 4], nodes[i]);
    }

    /* test search functionality */

    gr_node *result = gr_find_by_id(nodes[0], 5);
    if(result->id != 5)
        return EXIT_FIND_ERROR;
    
    /* test serialization */

    uint32_t buffer_size = 0;
    char *buffer = gr_serialize(nodes[0], &buffer_size); 
    if(buffer == NULL)
        return EXIT_FIND_ERROR;
}
