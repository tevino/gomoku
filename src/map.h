#ifndef MAP_H
#define MAP_H

#define MIN_WIDTH 5
#define MAX_WIDTH 'Z'
#define MAX_HEIGHT 100

typedef enum {NOTEXIST=-1, NOBODY=0, BLACK=1, WHITE=2} Player;

typedef enum {
  LEFT_RIGHT,
  TOP_DOWN,
  UPPER_LEFT_LOWER_RIGHT,
  LOWER_LEFT_UPPER_RIGHT
} Direction;

typedef struct Map {
  Player **m;
  int height;
  int width;
} Map;

Map *map_create(int width, int height);
Player map_at(Map *map, int x, int y);
int map_set(Map *map, int x, int y, Player p);
int map_center(Map *map, int *x, int *y);
int map_empty_at(Map *map, int x, int y);
void map_free(Map *map);
int is_valid_size(int width, int height);
int map_is_full(Map *map);
Player check_line_winner(Map *map, int sx, int sy, Direction dir);
Player map_find_winner(Map *map);

#endif
