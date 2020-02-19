#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "life.h"

#define WORKER_THREADS 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[WORKER_THREADS + 1];
int ids[WORKER_THREADS];
int progress = 0;
int iter = 0;

void run_game(cell *W, int N, cell *temp) {
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

void print_game(cell *W, int N, cell *temp) {
  printf("initial state:\n");
  print_grid(W, N);

  run_game(W, N, temp);

  printf("\nfinal state:\n");
  print_grid(W, N);
}

void *worker(void *arg) {
  int id = *((int *) arg);
  int done;
  printf("worker thread %d initialized\n", id);

  for (int i = 0; i < 5; i++) {
    pthread_mutex_lock(&mutex);

    progress += 1;
    printf("id: %d, progress: %d, iter: %d\n", id, progress, iter);

    if (progress == WORKER_THREADS) {
      printf("done in id %d\n", id);
      progress = 0;
      iter += 1;
      pthread_cond_broadcast(&cond);
    } else {
      pthread_cond_wait(&cond, &mutex);
    }

    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main() {
  // int N = 11;
  // cell *W = malloc(N * N * sizeof(cell));
  // cell *temp = malloc(N * N * sizeof(cell));
  // border_grid(W, N);
  // print_game(W, N, temp);

  for (int i = 0; i < WORKER_THREADS; i++) {
    ids[i] = i;
    if (pthread_create(&thread_pool[i], NULL, worker, (void *)(&ids[i]))) {
      printf("failure creating worker thread\n");
      exit(1);
    }
  }

  pthread_exit(NULL);
}
