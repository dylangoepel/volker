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
#define alloc_ret(ptr, size, ret) { \
    ptr = malloc(size); \
    if(ptr == NULL) \
        return NULL; \
    }

#define realloc_or_fail_ret(ptr, size, retval) { \
    void *newptr = realloc(ptr, size); \
    if(newptr == NULL) { \
        free(ptr); \
        return retval; \
    } \
    ptr = newptr; \
    }

#define ensure_space_ret(ptr, size, used, additional, ret) { \
    if(*size - used <= additional) { \
        realloc_or_fail_ret(ptr, *size + MEM_ALLOC_BLOCK, ret); \
        *size += MEM_ALLOC_BLOCK; \
    } }


#endif
