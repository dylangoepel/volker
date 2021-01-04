/* this header file defines constants, macros and functions related to memory allocation */

#ifndef MEM_H_
#define MEM_H_

#include <stdlib.h>
#include "fail.h"

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

#define realloc_or_fail(ptr, size) { \
    void *newptr = realloc(ptr, size); \
    if(newptr == NULL) { \
        fail_free(ptr); \
    } \
    ptr = newptr; \
    }

#define ensure_space(ptr, size, used, additional) { \
    if(*size - used <= additional) { \
        realloc_or_fail(ptr, *size + MEM_ALLOC_BLOCK); \
        *size += MEM_ALLOC_BLOCK; \
    } }

#endif
