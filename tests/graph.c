#include "graph/graph.h"
#include "graph/serial.h"

#include <stdlib.h>
#include <stdio.h>
#define EXIT_FIND_ERROR 1
#define EXIT_SERIALIZE_ERROR 2
#define EXIT_LINEARIZE_SIZE_ERROR 3
#define EXIT_LINEARIZE_SUM_ERROR 4
#define EXIT_DCONNECT_ERROR 5


int main() {
    gr_node *nodes[6];

    /* build graph */

    for(int i = 0; i < 6; ++i) {
        nodes[i] = gr_new(i, 0);
    }

    for(int i = 0; i < 6; ++i) {
        gr_connect(nodes[i > 0 ? i - 1 : 4], nodes[i]);
    }
  
    /* test gr_linearize */
    uint32_t size;
    gr_node **linear = gr_linearize(nodes[0], &size);
    if(size != 6) {
        if(linear != NULL)
            free(linear);
        return EXIT_LINEARIZE_SIZE_ERROR;
    }

    uint32_t idsum = 0;
    for(int i = 0; i < 6; ++i) {
        idsum += linear[i]->id;
    }
    free(linear);
    if(idsum != 15) {
        return EXIT_LINEARIZE_SUM_ERROR;
    }

    /* test search functionality */ 

    gr_node *result = gr_find_by_id(nodes[0], 5);
    if(result->id != 5)
        return EXIT_FIND_ERROR;

    /* test gr_dconnect */
    gr_dconnect(nodes[4], nodes[5]);
    result = gr_find_by_id(nodes[4], 5);
    if(result != NULL)
        return EXIT_DCONNECT_ERROR;
    
    /* test serialization */

    uint32_t buffer_size = 0;
    char *buffer = gr_serialize(nodes[0], &buffer_size); 
    if(buffer == NULL)
        return EXIT_SERIALIZE_ERROR;

    return 0;
    
}
