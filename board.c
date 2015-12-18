#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>
#include "popen2.h"
#include "map.h"

#define LINE_WIDTH 100
#define MIN_WIDTH 5


char *EMPTY_STR = ". ";
char *BLACK_STR = "X ";
char *WHITE_STR = "O ";


int letter_no(char c){
  c = toupper(c);
  if (c >= (int)'A' && c <= (int)'Z'){
    return c - (int)'A';
  }
  return -1;
}

int read_move(FILE *f, int *x, int *y){
  char buf[LINE_WIDTH];
  if (fgets(buf, sizeof(buf), f) == NULL){
    return -1;
  }

  char xc;
  if (sscanf(buf, "%c%d\n", &xc, y) != 2){
    return -1;
  };
  
  *x = letter_no(xc);

  return 0;
}

int width = 15;
int height = 15;

Player next = BLACK;

int new_move(Map *map, int x, int y, Player *winner){
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
    puts("      board black_cmd white_cmd [width] [height]");
    puts("      Default size: 15x15");
    return 1;
  }

  char *b_cmd = argv[1];
  char *w_cmd = argv[2];
    
  if (argc == 5) {
    width = atoi(argv[3]);
    height = atoi(argv[4]);
    if (width < MIN_WIDTH){
      fprintf(stderr, "Invalid width: %s", argv[3]);
      return 1;
    }
    
    if (height < MIN_WIDTH) {
      fprintf(stderr, "Invalid height: %s", argv[4]);
      return 1;
    }
  }

  Map *map = map_create(width, height);

  int b_fd, w_fd;
  pid_t b_pid, w_pid;
  FILE *b_f, *w_f;
  
  printf("launching black AI: %s\n", b_cmd);
  
  b_pid = popen2(&b_fd, (char *[]){b_cmd, "-first", NULL});

  if (b_pid < 0){
    fprintf(stderr, "Black[%s] AI failed.\n", b_cmd);
    return 1;
  }
  b_f = fdopen(b_fd, "w+");
  
  printf("launching white AI: %s\n", w_cmd);
  w_pid = popen2(&w_fd, (char *[]){w_cmd, NULL});
  if (w_pid < 0){
    fprintf(stderr, "White[%s] AI failed.\n", w_cmd);
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
    puts("Waiting for black...");
    if (read_move(b_f, &x, &y) < 0){
      puts("Invalid output of black.");
      winner = WHITE;
      break;
    }
    
    if (new_move(map, x, y, &winner) != 0){
      printf("Invalid move of black: %c%d\n", 'A' + x, y);
      winner = WHITE;
      break;
    }
    printf("Black: %c%d\n", 'A' + x, y);
    fprintf(w_f, "%c%d\n", 'A' + x, y);

    print_board(map);
    // TODO: winner?


    puts("Waiting for white...");
    if (read_move(w_f, &x, &y) < 0){
      puts("Invalid output of white.");
      winner = BLACK;
      break;
    }

    if (new_move(map, x, y, &winner) != 0) {
      printf("Invalid move of white: %c%d\n", 'A'+x, y);
      winner = BLACK;
      break;
    }
    printf("White: %c%d\n", 'A' + x, y);    
    fprintf(b_f, "%c%d\n", 'A' + x, y);
    
    print_board(map);
    // TODO: winner?
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
