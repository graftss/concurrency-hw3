#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

struct timespec ts;
struct timeval tv;

long get_real_time() {
  gettimeofday(&tv, NULL);
  return 1e9 * tv.tv_sec + 1e3 * tv.tv_usec;
}

long get_cpu_time() {
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  return 1e9 * ts.tv_sec + ts.tv_nsec;
}

// return units are seconds
float diff_nano_times(long start, long finish) {
  return (double) (finish - start) / 1e9;
}

void print_real_time_since(long start, char* label) {
  float diff = diff_nano_times(start, get_real_time());
  printf("%s: %.8f seconds\n", label, diff);
}
