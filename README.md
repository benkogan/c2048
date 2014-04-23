u2048
=====

2048, as you like it. Name based off of earlier ideas; could be better! :)

## Usage

Just `git clone` the repo, `cd u2048` into it, `make`, and run `./u2048` of course. Tested on Mac OS X. Will test on Arch Linux, but probably will need a few changes before it'll compile.

## Method

There are a few C implementations of 2048. I wanted to come up with my own, so I didn't look at any others until I finished mine.

The fun of my implementation, as compared to others, is that there are four boards but only one set of board values. Each board is simply an array of pointers to the same values arranged in a different orientation. The left-facing board is the "canonical" board, i.e. the one the user sees. Through this method, I was able to avoid code duplication in functions like `move(*board[SIZE][SIZE])`. I also didn't need functions to (expensively) rearrange the values on the board since there is no one true board.
