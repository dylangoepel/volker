#include "graph/graph.h"
#include "graph/serial.h"

#include <stdlib.h>
#include <stdio.h>
#define EXIT_FIND_ERROR 1
#define EXIT_SERIALIZE_ERROR 2
#define EXIT_LINEARIZE_SIZE_ERROR 3
#define EXIT_LINEARIZE_SUM_ERROR 4

int main() {
    gr_node *nodes[6];

    /* build graph */

    for(int i = 0; i < 6; ++i) {
        nodes[i] = gr_new(i, 0);
    }

    for(int i = 0; i < 6; ++i) {
        gr_connect(nodes[i > 0 ? i - 1 : 4], nodes[i]);
    }

    /*test gr_dconnect, it works ;)*/
    
    gr_node *temp1 = gr_new(10, 1);
    gr_node *temp2 = gr_new(11, 2);
    gr_node *temp3 = gr_new(12, 3);

    gr_connect(temp1, temp2);
    gr_connect(temp1, temp3);

    gr_node *res = gr_find_by_id(temp1, 11);
    if(res->id == 11)
      printf("%s","Yap");
    
    gr_dconnect(temp1, temp3);
    // now it should crash :)
    gr_node *res2 = gr_find_by_id(temp1, 11);
    if(res2->id == 11)
      printf("%s","Yes");
    
    /*just be careful if you dconnect two nodes and than later find_by_id the dconnected one*/ 
    
      
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
        idsum += (*(linear + i * sizeof(void*)))->id;
    }
    free(linear);
    if(idsum != 15) {
        return EXIT_LINEARIZE_SUM_ERROR;
    }

    /* test search functionality */

    gr_node *result = gr_find_by_id(nodes[0], 5);
    if(result->id != 5)
        return EXIT_FIND_ERROR;
    
    /* test serialization */

    uint32_t buffer_size = 0;
    char *buffer = gr_serialize(nodes[0], &buffer_size); 
    if(buffer == NULL)
        return EXIT_SERIALIZE_ERROR;

    return 0;
}
