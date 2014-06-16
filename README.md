c2048
=====

2048 in C.

## Usage

Setup:

```sh
$ git clone https://github.com/benkogan/c2048.git
$ cd c2048/
$ make
$ ./c2048
```

Quit with `q` or `ctrl-c`. Tested on Mac OS X and Arch Linux. C99.

## About

Where my implementation seems to differ most from others that I've seen is that
there are four boards, but only one set of board values stored in memory. Each
board is simply an array of pointers to the same values but arranged in a
different orientation. The left-facing board is the "canonical board," as this
is the board that the user sees.

The `slide(int *b[SIZE][SIZE])` function slides the tiles on a given board to
the left per the game mechanics. Thus, when `slide` is used on a given board,
the tiles on the canonical board will reflect this slide in the given board's
namesake direction. For example, `slide(boardUp)` will be seen to slide the
tiles up on the canonical board by sliding the tiles to the left on `boardUp`.

Through this method, I was able to avoid code duplication in board-manipulating
functions. I also didn't need functions to expensively rearrange the values on
each board individually after a move since there is no one true board.

## License

MIT

