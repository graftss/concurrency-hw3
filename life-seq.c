#include <stdio.h>
#include <stdlib.h>
#include "life.h"
#include "timing.h"

cell temp[N * N];

void run_game(cell *W) {
  cell *in = W;
  cell *out = temp;

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    in = i % 2 ? temp : W;
    out = i % 2 ? W : temp;

    if (all_dead(in) && in == temp) {
      clear_grid(W);
    }

    next_grid_state(in, out);
  }

  if (out == temp) {
    copy_grid(temp, W);
  }
}

void print_game(cell *W) {
  if (PRINT_GRID) {
    printf("initial state:\n");
    print_grid(W);
  }

  run_game(W);

  if (PRINT_GRID) {
    printf("\nfinal state:\n");
    print_grid(W);
  }
}

int main() {
  cell *W = malloc(N * N * sizeof(cell));
  border_grid(W);

  long start = get_real_time();
  print_game(W);
  print_real_time_since(start, "sequential");

  printf("cells alive: %d\n", num_alive(W));
}
