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
