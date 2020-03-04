#include <stdio.h>
#include <stdlib.h>
#include "life.h"
#include "timing.h"

cell temp[N * N];
cell grid[N * N];

void run_game(cell *grid) {
  cell *in = grid;
  cell *out = temp;

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    in = i % 2 ? temp : grid;
    out = i % 2 ? grid : temp;

    if (all_dead(in) && in == temp) {
      clear_grid(grid);
    }

    for (int i = 0; i < N * N; i++) {
      out[i] = next_cell_state(in, i);
    }
  }

  if (out == temp) {
    copy_grid(temp, grid);
  }
}

void print_game(cell *grid) {
  if (PRINT_GRID) {
    printf("initial state:\n");
    print_grid(grid);
  }

  run_game(grid);

  if (PRINT_GRID) {
    printf("\nfinal state:\n");
    print_grid(grid);
  }
}

int main() {
  border_grid(grid);

  long start = get_real_time();
  print_game(grid);
  print_real_time_since(start, "sequential");

  printf("cells alive: %d\n", num_alive(grid));
}
