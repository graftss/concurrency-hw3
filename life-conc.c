#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "life.h"

#define WORKER_THREADS 8

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pthread_t thread_pool[WORKER_THREADS + 1];
int ids[WORKER_THREADS];

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
  printf("worker thread %d initialized\n", id);
  pthread_exit(NULL);
}

void *master() {
  printf("master thread initialized\n");
  pthread_exit(NULL);
}

int main() {
  int N = 11;
  cell *W = malloc(N * N * sizeof(cell));
  cell *temp = malloc(N * N * sizeof(cell));

  for (int i = 0; i < WORKER_THREADS; i++) {
    ids[i] = i;
    if (pthread_create(&thread_pool[i], NULL, worker, (void *)(&ids[i]))) {
      printf("failure creating worker thread\n");
      exit(1);
    }
  }

  if (pthread_create(&thread_pool[WORKER_THREADS], NULL, master, NULL)) {
    printf("failure creating master thread\n");
    exit(1);
  }

  pthread_exit(NULL);
  // border_grid(W, N);
  // print_game(W, N, temp);
}
