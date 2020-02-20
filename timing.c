#include <stdio.h>
#include <stdlib.h>
#include "timing.h"


long get_nano_time() {
  struct timespec *tp = malloc(sizeof(struct timespec));
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, tp);
  return 1e9 * tp->tv_sec + tp->tv_nsec;
}

// return units are seconds
float diff_nano_times(long start, long finish) {
  return (double) (finish - start) / 1e9;
}

void print_time_since(long start, char* label) {
  float diff = diff_nano_times(start, get_nano_time());
  printf("%s: %.8f seconds\n", label, diff);
}
