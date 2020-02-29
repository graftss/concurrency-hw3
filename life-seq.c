#include <stdio.h>
#include <stdlib.h>
#include "life.h"
#include "timing.h"

void run_game(cell *W, int N) {
  cell temp[N * N];

  cell *in = W;
  cell *out = temp;

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    in = i % 2 ? temp : W;
    out = i % 2 ? W : temp;

    if (all_dead(in, N)) {
      if (in == temp) clear_grid(W, N);
    }

    next_grid_state(in, N, out);
  }

  if (out == temp) {
    copy_grid(temp, N, W);
  }
}

void print_game(cell *W, int N) {
  if (PRINT_GRID) {
    printf("initial state:\n");
    print_grid(W, N);
  }

  run_game(W, N);

  if (PRINT_GRID) {
    printf("\nfinal state:\n");
    print_grid(W, N);
  }
}

int main() {
  int N = GRID_SIZE;

  cell *W = malloc(N * N * sizeof(cell));
  border_grid(W, N);

  long start = get_nano_time();
  print_game(W, N);
  print_time_since(start, "sequential");

  printf("cells alive: %d\n", num_alive(W, N));
}
