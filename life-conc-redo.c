#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "life.h"
#include "timing.h"

#define WORKER_THREADS 4

pthread_t thread_pool[WORKER_THREADS];
int ids[WORKER_THREADS];
int min_index[WORKER_THREADS + 1];
int iter = 0;

cell grid[N * N];
cell temp[N * N];

void *worker(void *arg) {
  int id = *((int *) arg);
  cell *in, *out;

  // compute the source and target grids based on the
  // parity of the current iteration
  in = iter % 2 ? temp : grid;
  out = iter % 2 ? grid : temp;

  for (int idx = min_index[id]; idx < min_index[id + 1]; idx++) {
    // printf("%d", id);
    out[idx] = next_cell_state(in, idx);
  }

  pthread_exit(NULL);
}

void run_life_conc() {
  int i;

  if (PRINT_GRID) {
    printf("initial state:\n");
    print_grid(grid);
  }

  // compute the index of the grid that each thread will start at
  for (i = 0; i <= WORKER_THREADS; i++) {
    min_index[i] = i * N * N / WORKER_THREADS;
  }

  for (iter = 0; iter < MAX_ITERATIONS; iter++) {
    for (i = 0; i < WORKER_THREADS; i++) {
      ids[i] = i;
      if (pthread_create(&thread_pool[i], NULL, worker, (void *)(&ids[i]))) {
        printf("failure creating worker thread\n");
        exit(1);
      }
    }

    for (i = 0; i < WORKER_THREADS; i++) {
      pthread_join(thread_pool[i], NULL);
    }

    cell *out = iter % 2 ? grid : temp;
    if (all_dead(out) && out != grid) {
      clear_grid(grid);
      break;
    }
  }

  if (PRINT_GRID) {
    printf("final state:\n");
    print_grid(grid);
  }
}

int main() {
  border_grid(grid);

  long start = get_real_time();
  run_life_conc();
  print_real_time_since(start, "concurrent");

  printf("cells alive: %d\n", num_alive(grid));
}
