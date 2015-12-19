#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../map.h"

#define ERR(...) fprintf(stderr, __VA_ARGS__); return 1;

#define MIN_WIDTH 5
#define MAX_WIDTH 'Z'
#define MAX_HEIGHT 100

int is_valid_size(int width, int height) {
  if (width >= MIN_WIDTH && width < MAX_WIDTH &&
      height >= MIN_WIDTH && height < MAX_HEIGHT){
    return 1;
  }
  return 0;
}

int read_move(int *x, int *y) {
  char line[1024];
  fgets(line, sizeof(line), stdin);

  char x_c;
  sscanf(line, "%c%d\n", &x_c, y);

  *x = (int)(x_c - 'A');
  return 0;
}


#include <time.h>
int rand_int(int min, int max) {
  if (max - min <= 0){
    return -1;
  }

  return (rand() % max - min) + min;
}


typedef struct Moves {
  int x;
  int y;
  int is_root;

  struct Moves *pre;
  struct Moves *next;
} Moves;


Moves *other_moves;

Moves *moves_create(int x, int y) {
  Moves *moves = malloc(sizeof(Moves));
  if (x < 0 || y < 0) {
    moves->is_root = 1;
    moves->x = 0;
    moves->y = 0;
  } else {
    moves->x = x;
    moves->y = y;
  }
  moves->pre = NULL;
  moves->next = NULL;
  return moves;
}


Moves *moves_append(Moves *moves, int x, int y) {
  Moves *m = moves_create(x, y);
  m->pre = moves;
  moves->next = m;
  return m;
}


int next_move(Map *map, int *x, int *y) {
  if (other_moves->is_root) {  // me first
    map_center(map, x, y);
    return 0;
  }

  Moves *target_move = other_moves;

  // around opponent randomly
  while (1) {
    *x = target_move->x;
    *y = target_move->y;

    for (int i = 0; i < 4; i++) {
      int direction = rand_int(0, 4);
      switch (direction) {
      case 0:  // up
        *y -= 1;
        break;
      case 1:  // down
        *y += 1;
        break;
      case 2:  // left
        *x -= 1;
        break;
      case 3:  // right
        *x += 1;
        break;
      }

      if (map_empty_at(map, *x, *y)) {
        return 0;
      }
    }

    if (!target_move->is_root) {
      target_move = target_move->pre;
      continue;
    }

    break;
  }

  // trying an random empty point
  while (1) {
    *x = rand_int(0, map->width+1);
    *y = rand_int(0, map->height+1);
    if (map_empty_at(map, *y, *y)) return 0;
  }

  return 0;
}

void print_move(int x, int y) {
  printf("%c%d\n", 'A' + x, y);
}

int main(int argc, char *argv[]){
  srand(time(NULL));
  if (argc != 3 && argc != 4) {
    ERR("Usage: %s width height [-first]", argv[0]);
  }

  int first = 0;
  if (argc > 3) {
    first = strcmp(argv[3], "-first") == 0;
  }

  int width = atoi(argv[1]);
  int height = atoi(argv[2]);

  if (!is_valid_size(width, height)) {
    ERR("Invalid size: %d x %d", width, height);
  }


  Map *map = map_create(width, height);

  Player other = BLACK;
  Player me = WHITE;


  if (first) {
    me = BLACK;
    other = WHITE;
  }

  other_moves = moves_create(-1, -1);
  int x, y;
  while (1) {
    if (!first) {
      read_move(&x, &y);
      map_set(map, x, y, other);
      other_moves = moves_append(other_moves, x, y);
    }

    next_move(map, &x, &y);
    map_set(map, x, y, me);
    print_move(x, y);

    first = 0;
  }

  map_free(map);
}
