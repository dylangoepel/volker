#define _GNU_SOURCE
#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <string.h> 
#include <pthread.h> 
#include <errno.h> 

#include "tpool/tpool.h" 

#define ERROR_FULLQUEUE -2

static inline int _tpool_queue_len(tpool_queue *q) {
    tpool_queue *c;
    int len = 0;

    for(c = q; c != NULL; c = c->tail)
        ++len;

    return len;
}

static inline tpool_queue *_tpool_queue_last(tpool_queue *q) {
    tpool_queue *c;

    for(c = q; c->tail != NULL; c = c->tail) {}

    return c;
}

static inline int _tpool_queue_thread_count(tpool_queue *q) {
    tpool_queue *c;
    int thread_count = 0;

    for(c = q; c != NULL; c = c->tail) {
        if(c->has_thread)
            ++thread_count;
    }

    return thread_count;
}

int tpool_init(tpool_t *pool, int thread_count, int queue_size) {
    pool->max_queue_size = queue_size;
    pool->max_thread_count = thread_count;
    if(pthread_mutex_init(&pool->lock, NULL) < 0)
        return -1;
    pool->queue = NULL;
    return 0;
}

int tpool_add_work(tpool_t *pool, void *(*routine)(void *), void *arg) {
    // TODO: proper error handling
    tpool_queue *new;
    pthread_mutex_lock(&pool->lock);

    if(_tpool_queue_len(pool->queue) >= pool->max_queue_size)
        return ERROR_FULLQUEUE;

    new = malloc(sizeof(tpool_queue));
    if(new == NULL)
        return -1;

    // connect new element to queue
    _tpool_queue_last(pool->queue)->tail = new;

    // initialize new element
    new->routine = routine;
    new->arg = arg;
    new->has_thread = 0;
    new->tail = NULL;

    pthread_mutex_unlock(&pool->lock);

    tpool_update(pool);
    return 0;
}

int tpool_destroy(tpool_t *pool, char join) {
    tpool_queue *c, *d;

    pthread_mutex_lock(&pool->lock);

    // signalize shutdown to possible tpool updater thread
    pool->max_queue_size = pool->max_thread_count = 0;

    // iterate through queue and free
    while(c != NULL) {
        c = pool->queue;

        if(join && c->has_thread) { // if supposed to wait for threads to end
            pthread_join(c->thread, NULL);
        }

        pool->queue = c->tail;
        free(c);
    }

    pthread_mutex_unlock(&pool->lock);
    return 0;
}

int tpool_update(tpool_t *pool) {
    tpool_queue *c;

    // cleanup queue
    for(c = pool->queue; c->tail != NULL; c = c->tail) {
        if(!c->tail->has_thread)
            continue;
        if(pthread_tryjoin_np(c->tail->thread, NULL) != EBUSY) {
            tpool_queue *tmp;
            tmp = c->tail;
            c->tail = c->tail->tail;
            free(tmp);
        }
    }

    // run 
    int thread_count = _tpool_queue_thread_count(pool->queue);
    if(thread_count < pool->max_thread_count) {
        for(c = pool->queue; (c != NULL) &&
                (thread_count != pool->max_thread_count); c = c->tail) {
            if(c->has_thread)
                continue;
            pthread_create(&c->thread, NULL, c->routine, c->arg);
            c->has_thread = 1;
            thread_count++;
        }
    }

    return 0;
}
