#include <stdio.h>
#include <stdlib.h>

#include "tpool/tpool.h"
#include "benchmark.h"

#define NUM_THREADS 20
#define MAX_QUEUE 40

void aufg(void *arg){
  int a = (int)arg;
  a++;
}


int main(){

  benchmark ben;

  bm_start(&ben);
  
  tpool_t test;
  tpool_init(&test, NUM_THREADS, MAX_QUEUE);

  for(int i = 0; i < 30; ++i){
    tpool_add_work(test, aufg, (void *)(uintptr_t)i);
  }

  tpool_destroy(test, 1);

  bm_end(&ben);
  bm_write(&ben, "created tpool with 20 Threads ");
}
