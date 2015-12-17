board: board.c popen2.c
	cc -std=c99 -Wall -o $@ $^
