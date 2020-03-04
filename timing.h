#include <time.h>
#include <sys/time.h>

long get_real_time();
long get_cpu_time();
float diff_nano_times(long start, long finish);
void print_real_time_since(long start, char* label);
