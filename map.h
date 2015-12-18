#ifndef MAP_H
#define MAP_H

typedef struct Map {
  int **m;
  int height;
  int width;
} Map;

typedef enum {NOTEXIST=-1, NOBODY=0, BLACK=1, WHITE=2} Player;

Map *init_map(int width, int height);
Player map_at(Map *map, int x, int y);
int map_set(Map *map, int x, int y, Player p);

#endif
