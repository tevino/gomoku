#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *EMPTY_STR = ". ";
char *BLACK_STR = "X ";
char *WHITE_STR = "O ";

int width = 15;
int height = 15;
int first = 0;

typedef enum {NOBODY=0, BLACK = 1, WHITE = 2} Winner;

typedef struct _Point {
    int x;
    int y;
}Point;

int **initialMap(int width, int height) {
    int **map = malloc(sizeof(int *)*width);
    for (int ii=0; ii<width; ii++) {
        *(map+ii) = malloc(sizeof(int)*height);
        memset(*(map+ii), NOBODY, sizeof(int)*height);
    }
    return map;
}

void print_board(int **map){
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (map[x][y]){
                case BLACK:
                    printf("%s", BLACK_STR); break;
                case WHITE:
                    printf("%s", WHITE_STR); break;
                default:
                    printf("%s", EMPTY_STR); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int updateBoard(Point p, int value, int **map) {
    if (map[p.x][p.y] != NOBODY) {
        puts("Invalidate move, try another one");
        return -1;
    }
    map[p.x][p.y] = value;
//    print_board(map);
    return 0;
}

int isBoardFull(int **map) {
    int isfull = 1;
    for (int ii=0; ii<width; ii++) {
        int exist = 0;
        for (int jj=0; jj<height; jj++) {
            if (map[ii][jj] == NOBODY) {
                isfull = 0;
                exist = 1;
                break;
            }
        }
        if (exist) {
            break;
        }
    }
    return isfull;
}

Point nextStep(Point p, int **map) {
    int x = p.x;
    int y = p.y;
    if (x < width) {
        x += 1;
    }else {
        x = 0;
    }
    if (y < height) {
        y += 1;
    }else {
        y = 0;
    }
    Point newp = {x, y};
    if (map[x][y] == NOBODY) {
        return newp;
    }
    if (isBoardFull(map)) {
        puts("NO EMPTY PLACE !");
        return newp;
    }
    newp.x += 1;
    newp.y += 1;
    return nextStep(newp, map);
}

void pushNextStep(int isBegin, Point cp, int **map) {
    Point p = {0, 0};
    if (isBegin == 1) {
        updateBoard(p, BLACK, map);
        return;
    }
    if (updateBoard(cp, first == 1 ? WHITE: BLACK, map) == 0) {
        p = nextStep(cp, map);
        if (updateBoard(p, first == 1 ? BLACK : WHITE, map) == 0) {
            fprintf(stdout, "%c%d\n", p.x+'A', p.y);
        };
    };
}

int main(int argc, char *argv[]) {
    if (argc >= 2) {
        if (strcmp(argv[1], "-first") == 0) {
            first = 1;
        }
    }
    if (argc >= 3) {
        width = atoi(argv[2]);
    }
    if (argc >= 4) {
        height = atoi(argv[3]);
    }
    
    int **map = initialMap(width, height);
    
    if (first) {
        Point zp = {0, 0};
        pushNextStep(first, zp, map);
    }

    char getsBuffer[10];
    int finished = 0;
    while (finished == 0) {
        memset(getsBuffer, 0, 10*sizeof(char));
        if (fgets(getsBuffer, 10, stdin) > 0) {
//            printf("receive : %s\n", getsBuffer);
            Point p = {getsBuffer[0]-'A', getsBuffer[1]-48};
            pushNextStep(0, p, map);
        }
//        sleep(1);
    }

	return 0;
}

