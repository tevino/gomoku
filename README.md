# Gomoku

[![Build Status](https://travis-ci.org/tevino/gomoku.svg)](https://travis-ci.org/tevino/gomoku)

## AI

- Accept command-line arguments:

`./AI board-width board-height [-first]`

The last `-first` is **optional**, which indicates if the AI is Black.
The black should output a move right after its launch,
otherwise it should read the opponent's move from `stdin`.

- Output its moves to the stdout, one move a line.

- Reading opponent's move from stdin, one move a line.


### I/O format

A move is represented by an upper-case letter indicates the X axis followed with a number indicates the Y axis.

e.g. `A0` means the upper left point on the board.

- **Remember to put a newline after every move.**

- **Do not output anything other than your moves to `stdin` or your opponent will win.**
- `stderr` is not used, you can print debugging information whenever you like to it.

### API for building robot

- [map.h](https://github.com/tevino/gomoku/blob/master/src/map.h)
- [utils.h](https://github.com/tevino/gomoku/blob/master/src/utils.h)

## Example robot


- [naughty_kid](https://github.com/tevino/gomoku/blob/master/src/robots/naughty_kid.c) Randomly make moves around opponent.
