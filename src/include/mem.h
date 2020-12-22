/* this header file defines constants, macros and functions related to memory allocation */

#ifndef MEM_H_
#define MEM_H_

#include <stdlib.h>

#ifndef MEM_ALLOC_BLOCK
#define MEM_ALLOC_BLOCK 1024
#endif

// declare a variable on the heap
#define _heapvar(ptr, type) \
    type *ptr; \
    { \
    ptr = malloc(sizeof(type)); \
    if(ptr == NULL) \
        return NULL; \
    }

// allocate and return NULL on error
#define alloc_null(ptr, size) { \
    ptr = malloc(size); \
    if(ptr == NULL) \
        return NULL; \
    }

#endif
