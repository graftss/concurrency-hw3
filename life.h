#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 200
#define MAX_ITERATIONS 100
#define SHOULD_PRINT 0

typedef char cell;
#define ALIVE 1
#define DEAD 0
#define alive(cell) ((cell) == ALIVE ? 1 : 0)
#define alive3(nbs, i, j, k) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]))
#define alive5(nbs, i, j, k, l, m) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]) + alive(nbs[l]) + alive(nbs[m]))

void copy_grid(cell *in, int N, cell *out);
void clear_grid(cell *W, int N);
void border_grid(cell *W, int N);
cell next_cell_state(cell *W, int N, int i);
void next_grid_state(cell *in, int N, cell *out);
int all_dead(cell *W, int N);
void print_grid(cell *W, int N);
