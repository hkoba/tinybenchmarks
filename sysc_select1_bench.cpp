#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "benchmark.h"

int main(int, char** argv, char**) {
  argv++;

  long N = *argv ? atol(*argv++) : 10000000;
  
  {
    Benchmark::RUsage start;
    struct timeval tv = {tv_sec: 0, tv_usec: 0};
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    for (long i = 0; i < N; i++) {
      int rc = select(1, &rfds, NULL, NULL, &tv);
      if (rc == -1) {
	perror("select"); exit(1);
      }
    }
    start.elapsed(stdout, N, "select(1,{stdin},NULL,NULL,{0,0})");
  }
}
