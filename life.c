#include <stdio.h>
#include <stdlib.h>
#include "life.h"

int top_l = 0;
int top_r = N - 1;
int bot_l = N * (N - 1);
int bot_r = N * N - 1;

void copy_grid(cell *in, cell *out) {
  for (int i = 0; i < N * N; i++) {
    out[i] = in[i];
  }
}

void clear_grid(cell *W) {
  for (int i = 0; i < N * N; i++) {
    W[i] = DEAD;
  }
}

void border_grid(cell *W) {
  int bot_l = N * (N - 1);

  clear_grid(W);

  for (int i = 0; i < N; i++) {
    W[i] = ALIVE;
    W[bot_l + i] = ALIVE;
    W[N * i] = ALIVE;
    W[N * (i + 1) - 1] = ALIVE;
  }
}

// returns the next value of the cell at index `i`
cell next_cell_state(cell *W, int i) {
  cell currentState = W[i];

  int aliveNeighbors =
      // top left corner
      i == top_l     ? alive3(W, i - 1, i + N, i + N + 1)
      // top right corner
    : i == top_r     ? alive3(W, i - 1, i + N - 1, i + N)
      // bottom left corner
    : i == bot_l     ? alive3(W, i - N, i - N + 1, i + 1)
      // bottom right corner
    : i == bot_r     ? alive3(W, i - N - 1, i - N, i - 1)
      // non-corner top edge
    : i <  top_r     ? alive5(W, i - 1, i + 1, i + N - 1, i + N, i + N + 1)
      // non-corner left edge
    : i % N == 0     ? alive5(W, i - N, i - N + 1, i + 1, i + N, i + N + 1)
      // non-corner right edge
    : i % N == top_r ? alive5(W, i - N - 1, i - N, i - 1, i + N - 1, i + N)
      // non-corner bottom edge
    : i > bot_l      ? alive5(W, i - N - 1, i - N, i - N + 1, i - 1, i + 1)
      // non-edge
    : alive3(W, i - N - 1, i - N, i - N + 1) +
      alive5(W, i - 1, i + 1, i + N - 1, i + N, i + N + 1);

  switch (aliveNeighbors) {
    case 2: return currentState;
    case 3: return ALIVE;
    default: return DEAD;
  }
}

int all_dead(cell *W) {
  for (int i = 0; i < N * N; i++) {
    if (W[i] == ALIVE) return 0;
  }

  return 1;
}

int num_alive(cell *W) {
  int result = 0;

  for (int i = 0; i < N * N; i++) {
    if (W[i] == ALIVE) {
      result += 1;
    }
  }

  return result;
}

void print_grid(cell *W) {
  for (int i = 0; i < N * N; i++) {
    printf(alive(W[i]) ? "X" : ".");

    if (i % N == N - 1) {
      printf("\n");
    }
  }
}
