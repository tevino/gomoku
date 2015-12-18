board: board.c popen2.c map.c
	cc -std=c99 -Wall -o $@ $^
