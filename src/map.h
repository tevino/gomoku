#ifndef MAP_H
#define MAP_H

#define MIN_WIDTH 5
#define MAX_WIDTH 'Z'
#define MAX_HEIGHT 100

typedef struct Map {
  int **m;
  int height;
  int width;
} Map;

typedef enum {NOTEXIST=-1, NOBODY=0, BLACK=1, WHITE=2} Player;

Map *map_create(int width, int height);
Player map_at(Map *map, int x, int y);
int map_set(Map *map, int x, int y, Player p);
int map_center(Map *map, int *x, int *y);
int map_empty_at(Map *map, int x, int y);
void map_free(Map *map);
int is_valid_size(int width, int height);

#endif
