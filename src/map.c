#include <stdlib.h>
#include "map.h"

// create a new map with every point as NOBODY.
Map *map_create(int width, int height){
  int **m = calloc(width, sizeof(int *));
  for (int i = 0; i < width; i++) {
    m[i] = calloc(height, sizeof(int));
  }
  Map *map = malloc(sizeof(Map));
  map->height = height;
  map->width = width;
  map->m = m;

  return map;
}

// get player at x, y on map.
// return NOTEXIST if position invalid
Player map_at(Map *map, int x, int y) {
  if (map == NULL) return NOTEXIST;

  if (x >=0 && x < map->width && y >= 0 && y < map->height){
    return map->m[x][y];
  } else {
    return NOTEXIST;
  }
}

// set player at x, y on map.
// return 0 if the position is NOBODY or NOTEXIST else 1
int map_set(Map *map, int x, int y, Player p) {
  if (map == NULL) return -1;

  if (x >=0 && x < map->width && y >= 0 && y < map->height){
    Player before = map->m[x][y];
    map->m[x][y] = p;
    return (before == NOTEXIST || before == NOBODY)? 0 : 1;
  }
  return -1;
}

int map_center(Map *map, int *x, int *y) {
  if (map != NULL) {
    *x = map->width / 2 + 1;
    *y = map->height / 2 + 1;
    return 0;
  }
  return -1;
}

int map_empty_at(Map *map, int x, int y) {
  if (map == NULL) return -1;

  return map_at(map, x, y) == NOBODY;
}

void map_free(Map *map){
  if (map == NULL) return;

  for (int x = 0; x < map->width; x++){
    free(map->m[x]);
  }
  free(map->m);
  free(map);
}

int is_valid_size(int width, int height) {
  if (width >= MIN_WIDTH && width < MAX_WIDTH &&
      height >= MIN_WIDTH && height < MAX_HEIGHT){
    return 1;
  }
  return 0;
}

int map_is_full(Map *map) {
  for (int x = 0; x < map->width; x++) {
    for (int y = 0; y < map->height; y++) {
      if (map_empty_at(map, x, y)) return 0;
    }
  }

  return 1;
}

// find continuously 5 with given start point and direction
Player check_line_winner(Map *map, int sx, int sy, Direction dir) {
  int black_count = 0;
  int white_count = 0;

  int x = sx;
  int y = sy;
  Player pos;

  do {
    pos = map_at(map, x, y);

    switch (pos) {
      case BLACK: black_count++; break;
      case WHITE: white_count++; break;
      default:
        black_count = 0;
        white_count = 0;
        break;
    }

    if (black_count >= 5) return BLACK;
    if (white_count >= 5) return WHITE;

    switch (dir) {
      case LEFT_RIGHT: x++; break;
      case TOP_DOWN: y++; break;
      case LOWER_LEFT_UPPER_RIGHT:
        x++;
        y--;
        break;
      case UPPER_LEFT_LOWER_RIGHT:
        x++;
        y++;
        break;
    }
  } while (pos != NOTEXIST);

  return NOTEXIST;
}

Player map_find_winner(Map *map) {
  Player w = NOTEXIST;
  // -----
  for (int y = 0; y < map->height; y++) {
    w = check_line_winner(map, 0, y, LEFT_RIGHT);
  }
  if (w != NOTEXIST) return w;

  // |
  // |
  // |
  // |
  // |
  for (int x = 0; x < map->width; x++) {
    w = check_line_winner(map, x, 0, TOP_DOWN);
  }
  if (w != NOTEXIST) return w;


  // \
  //  \
  //   \
  //    \
  //     \
  //
  for (int x = 0; x < map->width; x++) {
    w = check_line_winner(map, x, 0, UPPER_LEFT_LOWER_RIGHT);
  }
  if (w != NOTEXIST) return w;
  for (int y = 0; y < map->height; y++) {
    w = check_line_winner(map, 0, y, UPPER_LEFT_LOWER_RIGHT);
  }
  if (w != NOTEXIST) return w;

  //     /
  //    /
  //   /
  //  /
  // /
  for (int y = 0; y < map->height; y++) {
    w = check_line_winner(map, 0, y, LOWER_LEFT_UPPER_RIGHT);
  }
  if (w != NOTEXIST) return w;
  for (int x = 0; x < map->width; x++) {
    w = check_line_winner(map, x, map->height - 1, LOWER_LEFT_UPPER_RIGHT);
  }
  if (w != NOTEXIST) return w;

  return NOTEXIST;
}
