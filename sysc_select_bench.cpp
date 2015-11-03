#include <pthread.h>
#include <stdlib.h>

#include "benchmark.h"
#include "exit_if_error.h"

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 400000000;
  
  {
    Benchmark::RUsage start;
    for (long i = 0; i < N; i++) {
      // HERE
    }
    start.elapsed(stdout, N, "THEME");
  }
}
