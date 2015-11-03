#include <pthread.h>
#include <stdlib.h>

#include "benchmark.h"

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 10000000;
  
  {
    Benchmark::RUsage start;
    struct timeval tv = {tv_sec: 0, tv_usec: 0};
    for (long i = 0; i < N; i++) {
      int rc = select(0, NULL, NULL, NULL, &tv);
      if (rc == -1) {
	perror("select"); exit(1);
      }
    }
    start.elapsed(stdout, N, "select()");
  }
}
