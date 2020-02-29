#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "life.h"
#include "timing.h"

#define WORKER_THREADS 4

pthread_t thread_pool[WORKER_THREADS];
int ids[WORKER_THREADS];
int iter = 0;
int iter_progress = 0;
int done = 0;

cell *grid = NULL;
cell *temp = NULL;
int N = GRID_SIZE;

void *worker(void *arg) {
  int id = *((int *) arg);
  cell *in, *out;

  // compute the indices of the grid that this thread will update
  int min_index = id * N * N / WORKER_THREADS;
  int max_index = (id + 1) * N * N / WORKER_THREADS - 1;

  // compute the source and target grids based on the
  // parity of the current iteration
  in = iter % 2 ? temp : grid;
  out = iter % 2 ? grid : temp;

  for (int idx = min_index; idx <= max_index; idx++) {
    // printf("%d", id);
    out[idx] = next_cell_state(in, N, idx);
  }

  pthread_exit(NULL);
}

void run_life_conc() {
  int i;

  if (PRINT_GRID) {
    printf("initial state:\n");
    print_grid(grid, N);
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

    // printf("ending iteration %d\n", iter);
  }

  // printf("\n\nfinal state:\n");
  // print_grid(grid, N);
  // printf("\n");
}

int main() {
  grid = malloc(N * N * sizeof(cell));
  temp = malloc(N * N * sizeof(cell));
  border_grid(grid, N);

  long start = get_nano_time();
  run_life_conc();
  print_time_since(start, "concurrent");

  printf("cells alive: %d\n", num_alive(grid, N));
}
