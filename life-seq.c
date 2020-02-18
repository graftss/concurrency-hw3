#include <stdio.h>
#include <stdlib.h>

typedef char cell;
#define ALIVE 1
#define DEAD 0
#define alive(cell) ((cell) == ALIVE ? 1 : 0)

#define alive3(nbs, i, j, k) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]))
#define alive5(nbs, i, j, k, l, m) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]) + alive(nbs[l]) + alive(nbs[m]))

void clear_grid(cell *W, int N) {
  for (int i = 0; i < N * N; i++) {
    W[i] = DEAD;
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
      i == top_l     ? alive3(nbs, 4, 6, 7)
    : i == top_r     ? alive3(nbs, 3, 5, 6)
    : i == bot_l     ? alive3(nbs, 1, 2, 4)
    : i == bot_r     ? alive3(nbs, 0, 1, 3)
    : i <  top_r     ? alive5(nbs, 3, 4, 5, 6, 7)
    : i % N == 0     ? alive5(nbs, 1, 2, 4, 6, 7)
    : i % N == top_r ? alive5(nbs, 0, 1, 3, 5, 6)
    : i > bot_l      ? alive5(nbs, 0, 1, 2, 3, 4)
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

void print_grid(cell *W, int N) {
  for (int i = 0; i < N * N; i++) {
    printf(alive(W[i]) ? "X" : ".");

    if (i % N == N - 1) {
      printf("\n");
    }
  }
}

int main() {
  int N = 10;
  cell *W0 = malloc(N * N * sizeof(cell));
  cell *W1 = malloc(N * N * sizeof(cell));
  clear_grid(W0, N);
  clear_grid(W1, N);

  W0[N] = ALIVE;
  W0[N + 1] = ALIVE;
  W0[N + 2] = ALIVE;

  print_grid(W0, N);
  next_grid_state(W0, N, W1);
  printf("\n");
  print_grid(W1, N);

}
