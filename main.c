#include <stdio.h>
#include "timing.h"

int main() {
  long start = get_nano_time();
  print_time_since(start, "test");
}
