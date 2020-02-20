#include <stdio.h>
#include <stdlib.h>
#include "life.h"

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
  printf("initial state:\n");
  print_grid(W, N);

  run_game(W, N);

  printf("\nfinal state:\n");
  print_grid(W, N);
}

void run_life_seq(int N) {
  cell *W = malloc(N * N * sizeof(cell));
  border_grid(W, N);

  print_game(W, N);
}

int main() {
  run_life_seq(50);
}
