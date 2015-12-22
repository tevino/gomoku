all: bin board naughty-kid jvcol ai_sm

bin:
	mkdir -p bin

board: board.c popen2.c map.c utils.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

naughty-kid: robots/naughty_kid.c map.c utils.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

jvcol: robots/jvcol.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^

ai_sm: robots/ai_sm.c
	${CC} -std=c99 -g -Wall -o bin/$@ $^
