#ifndef TPOOL_H
#define TPOOL_H
typedef struct tpool_work { 
  void        (*routine)(void *); 
  void        *arg; 
  struct tpool_work  *next; 
} tpool_work_t; 
 
typedef struct tpool { 
  int         num_threads; 
  int         max_queue_size; 
  pthread_t      *tpid; 
  tpool_work_t    *queue; 
  int         front, rear; 
  int         queue_closed;   
  int         shutdown;     
  pthread_mutex_t   queue_lock; 
  pthread_cond_t   queue_has_task; 
  pthread_cond_t   queue_has_space; 
  pthread_cond_t   queue_empty; 
} *tpool_t; 
 
void tpool_init(tpool_t *tpoolp,size_t num, size_t max_queue_size); 
 
int tpool_add_work(tpool_t tpool,void(*routine)(void *), void *arg); 
 
int tpool_destroy(tpool_t tpool,int finish);

#endif