#include <pthread.h>
#include <stdlib.h>

#include "benchmark.h"
#include "exit_if_error.h"

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 400000000;
  
  {
    pthread_mutex_t mutex;
    EXIT_IF_ERROR(pthread_mutex_init, (&mutex, NULL));

    Benchmark::RUsage start;
    for (long i = 0; i < N; i++) {
      EXIT_IF_ERROR(pthread_mutex_lock, (&mutex));
      EXIT_IF_ERROR(pthread_mutex_unlock, (&mutex));
    }
    start.elapsed(stdout, N, "single core, default mutex");    
  }
  
  return 0;
}
