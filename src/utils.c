#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

int rand_int(int min, int max) {
  if (max - min <= 0){
    return -1;
  }

  return (rand() % max - min) + min;
}

int read_move(FILE *f, int *x, int *y){
  char line[1024];
  if (fgets(line, sizeof(line), f) == NULL){
    return -1;
  }

  char x_c;
  if (sscanf(line, "%c%d\n", &x_c, y) != 2){
    return -1;
  };

  *x = (int)(toupper(x_c) - 'A');

  return 0;
}

void print_move(int x, int y) {
  printf("%c%d\n", 'A' + x, y);
}

