#ifndef TPOOL_H
#define TPOOL_H

#include <pthread.h>

typedef struct tpool_queue { 
  void        *(*routine)(void *); 
  void        *arg; 
  char  has_thread;
  pthread_t thread;
  struct tpool_queue    *tail;
} tpool_queue; 
 
typedef struct tpool { 
  int max_queue_size;
  int max_thread_count;
  tpool_queue *queue;
  pthread_mutex_t lock;
} tpool_t; 
 
int tpool_init(tpool_t *pool, int thread_count, int queue_size);
 
int tpool_add_work(tpool_t *pool,void*(*routine)(void *), void *arg); 
 
int tpool_destroy(tpool_t *tpool, char join);

int tpool_update(tpool_t *tpool);

#endif
