/* thread pool tests */

#include <unistd.h>
#include <stdio.h>
#include "tpool/tpool.h"

#define TASK_COUNT 20
#define MAX_QUEUE_SIZE 1000
#define MAX_THREAD_COUNT 10

static inline int _tpool_queue_len(tpool_queue *q) {
    tpool_queue *c;
    int len = 0;

    for(c = q; c != NULL; c = c->tail)
        ++len;

    return len;
}

void *nop(void *dat) {
    sleep(2);
    return NULL;
}

int main() {
    tpool_t pool;

    /* initialize pool */
    tpool_init(&pool, MAX_THREAD_COUNT, MAX_QUEUE_SIZE);

    /* assign tasks */
    for(int m = 0; m < 3; ++m) {
        for(int i = 0; i < TASK_COUNT; ++i) {
            tpool_add_work(&pool, &nop, NULL);
        }
        printf("[test_tpool) assigned %d tasks\n", m * TASK_COUNT);
        sleep(1);
    }

    while(pool.queue != NULL) {
        int num_tasks = _tpool_queue_len(pool.queue);
        printf("[test_tpool] %d task(s) left\n", num_tasks);
        tpool_update(&pool);
        sleep(1);
    }

    printf("[test_tpool] everything done. destroying thread pool. \n");
    tpool_destroy(&pool, 0);

    return 0;
}
