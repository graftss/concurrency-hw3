#include <stdio.h>
#include <stdlib.h>

#define N 5000
#define MAX_ITERATIONS 2
#define PRINT_GRID 0

typedef char cell;
#define ALIVE 1
#define DEAD 0
#define alive(cell) ((cell) == ALIVE ? 1 : 0)
#define alive3(nbs, i, j, k) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]))
#define alive5(nbs, i, j, k, l, m) (alive(nbs[i]) + alive(nbs[j]) + alive(nbs[k]) + alive(nbs[l]) + alive(nbs[m]))

void copy_grid(cell *in, cell *out);
void clear_grid(cell *W);
void border_grid(cell *W);
cell next_cell_state(cell *W, int i);
void next_grid_state(cell *in, cell *out);
int all_dead(cell *W);
int num_alive(cell *W);
void print_grid(cell *W);
