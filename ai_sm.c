#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ARG_WIDTH "-width"
#define ARG_HEIGHT "-height"
#define ARG_FIRST "-first"

#define BUF_SIZE 100

typedef struct _Board {
    int width;
    int height;
    int **pieces;
}Board;

typedef enum _Piece {
    PieceEmpty = 0,
    PieceSelf,
    PieceOpponent
}Piece;

typedef struct _Point {
    int x;
    int y;
}Point;

Board* initBoard(int width, int height);
void deinitBoard(Board *board);
void printBoard(Board *board);
void setBoard(Board *board, Point p, Piece value);
int isFinish(Board *board);
Point nextLocation(Board *board);
Point pointFromIn(char *in);

int main(int argc, char * argv[]) {
    int first = 0;
    int width = 15;
    int height = 15;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], ARG_WIDTH) == 0 && argc > i+1) {
            width = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], ARG_HEIGHT) == 0 && argc > i+1) {
            height = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], ARG_FIRST) == 0) {
            first = 1;
        }
    }
    
    Board *board = initBoard(width, height);
    if (first != 0) {
        Point p = nextLocation(board);
        setBoard(board, p, PieceSelf);
        fprintf(stdout, "%d%c", p.y, 'A'+p.x);
    }
    char rb[BUF_SIZE];
    int finish = 0;
    while (finish == 0) {
        memset(rb, 0, BUF_SIZE);
        if (fgets(rb, BUF_SIZE , stdin) > 0) {
            printf("read %s\n", rb);
            Point p = pointFromIn(rb);
            setBoard(board, p, PieceOpponent);
            finish = isFinish(board);
            if (finish == 0) {
                Point np = nextLocation(board);
                setBoard(board, np, PieceSelf);
                fprintf(stdout, "%d%c\n", np.y, 'A'+np.x);
                finish = isFinish(board);
            }
        }
        sleep(2);
    }
    
    deinitBoard(board);
    return 0;
}

Board* initBoard(int width, int height) {
    Board *board = (Board *)malloc(sizeof(Board));
    board->width = width;
    board->height = height;
    board->pieces = (int **)malloc(sizeof(int *) * height);
    for (int i = 0; i < height; i++) {
        *(board->pieces + i) = (int *)malloc(sizeof(int) * width);
        memset(*(board->pieces + i), 0, sizeof(int) * width);
    }
    return board;
}

void deinitBoard(Board *board) {
    if (board != NULL) {
        if (board->pieces != NULL) {
            for (int i = 0; i < board->height; i++) {
                free(*(board->pieces + i));
            }
            free(board->pieces);
            board->pieces = NULL;
        }
        free(board);
        board = NULL;
    }
}

void setBoard(Board *board, Point p, Piece value) {
    if (p.x >= 0 && p.x < board->width && p.y >= 0 && p.y < board->height) {
        board->pieces[p.y][p.x] = value;
    }
}

void printBoard(Board *board) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            printf("%d ", board->pieces[y][x]);
        }
        printf("\n");
    }
}

int isFinish(Board *board) {
    return 0;
}

Point nextLocation(Board *board) {
    Point p = {-1, -1};
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            if (board->pieces[y][x] == 0) {
                p.x = x;
                p.y = y;
                return p;
            }
        }
    }
    return p;
}

Point pointFromIn(char *in) {
    Point p = {0, 0};
    return p;
}

