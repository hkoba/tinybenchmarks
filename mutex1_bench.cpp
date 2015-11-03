#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define ERROR_EXIT(fn, args) error_exit(#fn, fn args)

inline static void error_exit(const char* func, int rc) {
  if (rc != 0) {
    printf("FAILED: %s: %s\n", func , strerror(rc));
    exit(1);
  }
}

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 400000000;
  
  {
    pthread_mutex_t mutex;
    ERROR_EXIT(pthread_mutex_init, (&mutex, NULL));

    Benchmark::RUsage start;
    for (long i = 0; i < N; i++) {
      ERROR_EXIT(pthread_mutex_lock, (&mutex));
      ERROR_EXIT(pthread_mutex_unlock, (&mutex));
    }
    start.elapsed(stdout, N, "single core, default mutex");    
  }
  
  return 0;
}