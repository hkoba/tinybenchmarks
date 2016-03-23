// -*- mode: C++; tab-width: 4 -*-
// $Id$
#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>

namespace Benchmark {

  double tv_diff_as_double(const timeval& now, const timeval& from) {
	timeval res;
	timersub(&now, &from, &res);
	return double(res.tv_sec) + 0.000001*(res.tv_usec);
  }

  struct RUsage : public rusage {
	int _rc;
	explicit RUsage() {
	  _rc = getrusage(RUSAGE_SELF, this);
	}
	void* is_ok() { return (void*)(_rc == 0); }
	void elapsed(FILE* out, long N, const char* title, const RUsage& now) const {
	  if (char* benchmark_id = getenv("BENCHMARK_ID")) {
		fprintf(out, "benchmark_id:%s\t", benchmark_id);
	  }
	  fprintf(out, "user_sec:%g\tsys_sec:%g\tN:%d\ttheme:%s\n"
			  , tv_diff_as_double(now.ru_utime, ru_utime)
			  , tv_diff_as_double(now.ru_stime, ru_stime)
			  , N
			  , title
			  );
	}
	void elapsed(FILE* out, long N, const char* title) const {
	  elapsed(out, N, title, RUsage());
	}
	void elapsed(FILE* out, const char* title, const RUsage& now) const {
	  fprintf(out, "user_sec:%g\tsys_sec:%g\ttheme:%s\n"
			  , tv_diff_as_double(now.ru_utime, ru_utime)
			  , tv_diff_as_double(now.ru_stime, ru_stime)
			  , title
			  );
	}
	void elapsed(FILE* out, const char* title) const {
	  elapsed(out, title, RUsage());
	}
  };
}

#endif//__BENCHMARK_H__
