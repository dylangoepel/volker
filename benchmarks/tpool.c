#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "tpool/tpool.h"
#include "benchmark.h"

#define NUM_THREADS 20
#define MAX_QUEUE 40

void *nop(void *arg){
  return NULL;
}


int main(){
  benchmark ben;
  tpool_t test;

  for(int i = 0; i < ITERATION_COUNT; ++i) {
      bm_start(&ben);
      tpool_init(&test, NUM_THREADS, MAX_QUEUE);
      bm_end(&ben);
      bm_write(&ben, "tpool_init");

      bm_start(&ben);
      for(int i = 0; i < 30; ++i){
        tpool_add_work(&test, &nop, NULL);
      }
      bm_end(&ben);
      bm_write(&ben, "tpool_add_work");

      bm_start(&ben);
      tpool_destroy(&test, 1);
      bm_end(&ben);
      bm_write(&ben, "tpool_destroy");
  }
}
