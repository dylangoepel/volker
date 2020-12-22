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

#define realloc_free_null(ptr, size) { \
    void *newptr = realloc(ptr, size); \
    if(newptr == NULL) { \
        free(ptr); \
        return NULL; \
    } \
    ptr = newptr; \
    }

#define realloc_null(ptr, size) { \
    void *newptr = realloc(ptr, size); \
    if(newptr == NULL) \
        return NULL; \
    ptr = newptr; \
    }

#endif
