#include "timing.h"
#include "life-seq.h"
#include "life-conc.h"

int main() {
  int N = 50;

  long start = get_nano_time();
  run_life_conc(N);
  print_time_since(start, "conc");

  start = get_nano_time();
  run_life_seq(N);
  print_time_since(start, "seq");
}
