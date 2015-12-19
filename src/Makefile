CC=cc

all: bin board naughty-kid

bin:
	mkdir -p bin

board: board.c popen2.c map.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

naughty-kid: AI/naughty_kid.c map.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^
