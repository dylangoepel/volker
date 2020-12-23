/* basic error management */

#define fail() { \
        return NULL; \
    }

#define fail_free(ptr) { \
    free(ptr); \
    fail(); \
    }
