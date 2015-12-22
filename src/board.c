#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include "popen2.h"
#include "map.h"
#include "utils.h"

char *EMPTY_STR = "· ";
char *BLACK_STR = "X ";
char *WHITE_STR = "O ";


int letter_no(char c){
  c = toupper(c);
  if (c >= (int)'A' && c <= (int)'Z'){
    return c - (int)'A';
  }
  return -1;
}

int width = 15;
int height = 15;

Player next = BLACK;

int check_winner(Map *map, Player *winner) {
  *winner = map_find_winner(map);
  if (*winner != NOTEXIST) {
    return 1;
  }

  if ((*winner == NOTEXIST) && map_is_full(map)) {
    // draw
    return 1;
  }

  return 0;
}

int new_move(Map *map, int x, int y){
  if (map_at(map, x, y) == NOBODY){
    map_set(map, x, y, next);
    next = next==BLACK?WHITE:BLACK;
    return 0;
  }

  return -1;
}


void print_board(Map *map){
  puts("");
  for (int y = -1; y < height; y++) {
    for (int x = -1; x < width; x++) {
      // legend of X (letters)
      if (y == -1) {
        // padding space
        if (x == -1) {
          printf("    ");
          continue;
        }

        printf("%c ", 'A' + x);
        continue;
      }

      // legend of Y (numbers)
      if (x == -1) {
        printf("%*d ", 3, y);
        continue;
      }

      switch (map_at(map, x, y)){
        case BLACK:
          printf("%s", BLACK_STR); break;
        case WHITE:
          printf("%s", WHITE_STR); break;
        default:
          printf("%s", EMPTY_STR); break;
      }
    }
    puts("");
  }
  puts("");
}


int main (int argc, char *argv[]){
  if (argc != 3 && argc != 5){
    puts("Usage:");
    puts("      board black_robot white_robot [width] [height]");
    puts("      Default size: 15x15");
    return 1;
  }

  char *b_cmd = argv[1];
  char *w_cmd = argv[2];

  if (argc == 5) {
    width = atoi(argv[3]);
    height = atoi(argv[4]);
    if (!is_valid_size(width, height)) {
      fprintf(stderr, "Invalid size: %s x %s", argv[3], argv[4]);
      return 1;
    }
  }

  char height_s[10], width_s[10];
  sprintf(height_s, "%d", height);
  sprintf(width_s, "%d", width);

  Map *map = map_create(width, height);

  int b_fd, w_fd;
  pid_t b_pid, w_pid;
  FILE *b_f, *w_f;

  printf("launching black Robot: %s\n", b_cmd);

  b_pid = popen2(&b_fd, (char *[]){b_cmd, width_s, height_s, "-first", NULL});

  if (b_pid < 0){
    fprintf(stderr, "Black[%s] robot failed.\n", b_cmd);
    return 1;
  }
  b_f = fdopen(b_fd, "w+");

  printf("launching white robot: %s\n", w_cmd);
  w_pid = popen2(&w_fd, (char *[]){w_cmd, width_s, height_s, NULL});
  if (w_pid < 0){
    fprintf(stderr, "White[%s] robot failed.\n", w_cmd);
    return 1;
  }
  w_f = fdopen(w_fd, "w+");

  if (b_f == NULL || w_f == NULL){
    puts("Open pipe failed.");
    return 1;
  }

  int x, y;
  Player winner = NOTEXIST;

  while (winner == NOTEXIST) {
    // Black's round
    puts("Waiting for black...");
    if (read_move(b_f, &x, &y) < 0){
      puts("Invalid output of black.");
      winner = WHITE;
      break;
    }

    if (new_move(map, x, y) != 0){
      printf("Invalid move of black: %c%d\n", 'A' + x, y);
      winner = WHITE;
      break;
    }
    printf("Black: %c%d\n", 'A' + x, y);
    fprintf(w_f, "%c%d\n", 'A' + x, y);

    print_board(map);
    if (check_winner(map, &winner)) break;

    // White's round
    puts("Waiting for white...");
    if (read_move(w_f, &x, &y) < 0){
      puts("Invalid output of white.");
      winner = BLACK;
      break;
    }

    if (new_move(map, x, y) != 0) {
      printf("Invalid move of white: %c%d\n", 'A'+x, y);
      winner = BLACK;
      break;
    }
    printf("White: %c%d\n", 'A' + x, y);
    fprintf(b_f, "%c%d\n", 'A' + x, y);

    print_board(map);
    if (check_winner(map, &winner)) break;
  }

  map_free(map);

  fclose(b_f);
  fclose(w_f);
  kill(w_pid, 9);
  kill(b_pid, 9);

  switch (winner) {
    case BLACK:
      puts("Black win."); break;
    case WHITE:
      puts("White win."); break;
    case NOBODY:
      puts("Draw."); break;
    case NOTEXIST:  // This should not happen
      puts("Winner not exist"); break;
  }
}
