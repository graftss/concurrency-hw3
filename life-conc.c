#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "life.h"

#define WORKER_THREADS 7

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_iter = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_done = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[WORKER_THREADS + 1];
int ids[WORKER_THREADS];
int iter_progress = 0;
int done = 0;

cell *grid = NULL;
cell *temp = NULL;
int N;

void *worker(void *arg) {
  int id = *((int *) arg);
  cell *in, *out;

  // compute the indices of the grid that this thread will update
  int min_index = id * N * N / WORKER_THREADS;
  int max_index = (id + 1) * N * N / WORKER_THREADS - 1;

  for (int i = 0; i < MAX_ITERATIONS; i++) {
    // compute the source and target grids
    in = i % 2 ? temp : grid;
    out = i % 2 ? grid : temp;

    for (int idx = min_index; idx <= max_index; idx++) {
      out[idx] = next_cell_state(in, N, idx);
    }

    pthread_mutex_lock(&mutex);

    iter_progress += 1;
    // printf("id: %d, iter_progress: %d, iter: %d\n", id, iter_progress, i);

    if (iter_progress == WORKER_THREADS) {
      // printf("done in id %d\n", id);
      iter_progress = 0;

      if (all_dead(out, N)) {
        if (in == temp) clear_grid(in, N);
        done = 1;
        pthread_cond_broadcast(&cond_done);
      } else {
        pthread_cond_broadcast(&cond_iter);
      }
    } else {
      pthread_cond_wait(&cond_iter, &mutex);
    }

    pthread_mutex_unlock(&mutex);
  }

  if (id == 0) {
    done = 1;
    pthread_cond_broadcast(&cond_done);
  }

  pthread_exit(NULL);
}

int main() {
  N = 17;
  grid = malloc(N * N * sizeof(cell));
  temp = malloc(N * N * sizeof(cell));
  border_grid(grid, N);

  for (int i = 0; i < WORKER_THREADS; i++) {
    ids[i] = i;
    if (pthread_create(&thread_pool[i], NULL, worker, (void *)(&ids[i]))) {
      printf("failure creating worker thread\n");
      exit(1);
    }
  }

  while (!done) {
    pthread_cond_wait(&cond_done, &mutex);
  }

  pthread_exit(NULL);
}
