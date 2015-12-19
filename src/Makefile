all: bin board naughty-kid jvcol

bin:
	mkdir -p bin

board: board.c popen2.c map.c utils.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

naughty-kid: AI/naughty_kid.c map.c utils.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

jvcol: AI/jvcol.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^
