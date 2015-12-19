#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define ERR(...) fprintf(stderr, __VA_ARGS__); return 1;

int rand_int(int min, int max);
int read_move(FILE *f, int *x, int *y);
void print_move(int x, int y);

#endif
