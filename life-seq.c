#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERATIONS 1000

typedef char cell;
#define ALIVE 1
#define DEAD 0
#define alive(cell) ((cell) == ALIVE ? 1 : 0)

#define alive3(nbs, i, j, k) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]))
#define alive5(nbs, i, j, k, l, m) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]) + alive(nbs[l]) + alive(nbs[m]))

void copy_grid(cell *in, int N, cell *out) {
  for (int i = 0; i < N * N; i++) {
    out[i] = in[i];
  }
}

void clear_grid(cell *W, int N) {
  for (int i = 0; i < N * N; i++) {
    W[i] = DEAD;
  }
}

void border_grid(cell *W, int N) {
  int bot_l = N * (N - 1);

  clear_grid(W, N);

  for (int i = 0; i < N; i++) {
    W[i] = ALIVE;
    W[bot_l + i] = ALIVE;
    W[N * i] = ALIVE;
    W[N * (i + 1) - 1] = ALIVE;
  }
}

// returns the next value of the cell at index `i`
cell next_cell_state(cell *W, int N, int i) {
  cell currentState = W[i];

  int top_l = 0;
  int top_r = N - 1;
  int bot_l = N * (N - 1);
  int bot_r = bot_l + N - 1;

  // some of these are garbage if the cell is near either end of the
  // array, but that shouldn't matter since we aren't using them...?
  cell nbs[8] = {
    W[i - N - 1], W[i - N], W[i - N + 1], W[i - 1],
    W[i + 1], W[i + N - 1], W[i + N], W[i + N + 1]
  };

  // layout of nbs:
  // 0 1 2
  // 3   4
  // 5 6 7

  int aliveNeighbors =
      // top left corner
      i == top_l     ? alive3(nbs, 4, 6, 7)
      // top right corner
    : i == top_r     ? alive3(nbs, 3, 5, 6)
      // bottom left corner
    : i == bot_l     ? alive3(nbs, 1, 2, 4)
      // bottom right corner
    : i == bot_r     ? alive3(nbs, 0, 1, 3)
      // non-corner top edge
    : i <  top_r     ? alive5(nbs, 3, 4, 5, 6, 7)
      // non-corner left edge
    : i % N == 0     ? alive5(nbs, 1, 2, 4, 6, 7)
      // non-corner right edge
    : i % N == top_r ? alive5(nbs, 0, 1, 3, 5, 6)
      // non-corner bottom edge
    : i > bot_l      ? alive5(nbs, 0, 1, 2, 3, 4)
      // non-edge
    :                  alive3(nbs, 0, 1, 2) + alive5(nbs, 3, 4, 5, 6, 7);

  switch (aliveNeighbors) {
    case 2: return currentState;
    case 3: return ALIVE;
    default: return DEAD;
  }
}

void next_grid_state(cell *in, int N, cell *out) {
  for (int i = 0; i < N * N; i++) {
    out[i] = next_cell_state(in, N, i);
  }
}

int all_dead(cell *W, int N) {
  for (int i = 0; i < N * N; i++) {
    if (W[i] == ALIVE) return 0;
  }

  return 1;
}

void print_grid(cell *W, int N) {
  for (int i = 0; i < N * N; i++) {
    printf(alive(W[i]) ? "X" : ".");

    if (i % N == N - 1) {
      printf("\n");
    }
  }
}

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

int main() {
  int N = 11;
  cell *W = malloc(N * N * sizeof(cell));
  border_grid(W, N);

  print_game(W, N);
}
