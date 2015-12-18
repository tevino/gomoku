CC=cc

board: board.c popen2.c map.c
	${CC} -std=c99 -Wall -o $@ $^
