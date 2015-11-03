#ifndef EXIT_IF_ERROR

#include <string.h>

#define EXIT_IF_ERROR(fn, args) do {int rc = fn args; if (rc) error_exit(#fn, rc); } while (0)

inline static void error_exit(const char* func, int rc) {
  printf("FAILED: %s: %s\n", func , strerror(rc));
  exit(1);
}

#endif
