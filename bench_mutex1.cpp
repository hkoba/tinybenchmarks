#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 400000000;
  
  {
    pthread_mutex_t mutex;
    int rc = 0;
    if ((rc = pthread_mutex_init(&mutex, NULL)) != 0) {
      printf("FAILED: pthread_mutex_init: %s\n"
	     , strerror(rc));
      exit(1);
    }
    Benchmark::RUsage start;
    for (long i = 0; i < N; i++) {
      if ((rc = pthread_mutex_lock(&mutex)) != 0) {
	printf("FAILED: pthread_mutex_lock: %s\n"
	       , strerror(rc));
	exit(1);
      }
      if ((rc = pthread_mutex_unlock(&mutex)) != 0) {
	printf("FAILED: pthread_mutex_unlock: %s\n"
	       , strerror(rc));
	exit(1);
      }
    }
    start.elapsed(stdout, N, "single core, default mutex"
		  , Benchmark::RUsage());    
  }
  
  return 0;
}
