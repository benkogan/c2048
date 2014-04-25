
/*
 * u2048.c
 *
 * Copyright (c) 2014 Ben Kogan <http://benkogan.com>
 * Gameplay based on 2048 by Gabriele Cirulli <http://gabrielecirulli.com/>
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#define GOAL 2048
#define SIZE 4

/*
 * boardLt is the canonical board; all other boards reflect LEFT as their
 * respective direction. To move a tile in any direction, it will be moved
 * LEFT on its respective directional board.
 *
 * For example, "up" on the canonical board is LEFT on boardUp.
 */
static int *(boardLt[SIZE][SIZE]), // left (canonical)
           *(boardRt[SIZE][SIZE]), // right
           *(boardUp[SIZE][SIZE]), // up
           *(boardDn[SIZE][SIZE]), // down
           *lastAdd = 0, // address of last tile added
            score   = 0,
            win     = false;

static const int QUIT = -1,
                 LOSE =  0,
                 WIN  =  1;

void cleanup() {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            free(boardLt[r][c]);
        }
    }
}

int quit(int op)
{
    char *msg = op == 1 ? "YOU WIN!": op == 0 ? "GAME OVER." : "QUIT";

    printf("\n\n%s\n", msg);
    cleanup();
    exit(0);
}

void terminate(int signum) { quit(QUIT); }

void initBoard()
{
    for (int r = 0; r < SIZE; r++) { // row for canonical board
        for (int c = 0; c < SIZE; c++) { // column for canonical board

            // create a new, empty tile
            int *tile = (int *)malloc(sizeof(int *));
            if (tile == NULL) {
                perror("malloc returned NULL");
                exit(1);
            }
            *tile = 0;

            // add same tile to equivalent location in all directional boards
            boardLt[r][c]        = tile;
            boardRt[r][SIZE-1-c] = boardLt[r][c];
            boardUp[SIZE-1-c][r] = boardLt[r][c];
            boardDn[c][SIZE-1-r] = boardLt[r][c];
        }
    }
}

void printBoard(int *board[SIZE][SIZE])
{
    printf("\n2048\n\nSCORE: %d\n\n", score);
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {

            if (*board[r][c]) {
                if (lastAdd == board[r][c])
                    printf("\033[036m%6d\033[0m", *board[r][c]); // with color
                else
                    printf("%6d", *board[r][c]);
            } else
                printf("%6s", ".");
        }
        printf("\n\n");
    }

    printf("\nMOVEMENT:\n   w\n a s d              ");
}

void addRandom()
{
    int r, c;
    do {
        r = rand()%4; // random number from 0 to 3
        c = rand()%4;
    } while (*boardLt[r][c] != 0);

    int random = rand()%2; // 0 or 1
    *boardLt[r][c] = 2 * random + 2; // 2 or 4
    lastAdd = boardLt[r][c];
}

/*
 * Slide all tiles left on directional board given as arguement
 */
bool slide(int *b[SIZE][SIZE])
{
    bool success = 0;  // true if something moves
    int  marker  = -1; // marks a cell one past location of a previous merge

    for (int r = 0; r < SIZE; r++) { // rows
        for (int c = 1; c < SIZE; c++) { // don't need to slide first col
            if (!(*b[r][c])) continue; // no tile

            // advance to proper merge target in new column (newc)
            int newc = c;
            while (newc && (!*b[r][newc-1] || *b[r][newc-1] == *b[r][c])) {
                newc--;
                if (*b[r][newc] == *b[r][c] || marker == newc) break;
            }

            // merge tile with target tile
            if (newc != c) {
                if (*b[r][newc]) {
                    score += *b[r][newc] += *b[r][c];
                    marker = newc+1;
                    if (*b[r][newc] == GOAL) win = true;
                } else {
                    *b[r][newc] += *b[r][c];
                }

                *b[r][c] = 0;
                success = true;
            }
        }
        marker = -1; // reset for next row
    }

    return success;
}

/*
 * Get and act on next move from user
 */
int move()
{
    bool success = false;

    char direction = getchar();
    switch(direction) {
        case 119:      // 'w' key; up
            success = slide(boardUp);
            break;
        case 97:       // 'a' key; left
            success = slide(boardLt);
            break;
        case 115:       // 's' key; down
            success = slide(boardDn);
            break;
        case 100:       // 'd' key; right
            success = slide(boardRt);
            break;
        case 113:       // 'q' key; quit
            quit(QUIT);
            break;
 //     default:
 //         success = false;
    }

    return success;
}

/*
 * Check for possible moves.
 *
 * Return: true if no moves are possible
 *         false if a move exists
 */
bool isFull()
{
    for (int r = SIZE-1; r >= 0; r--) {
        for (int c = SIZE-1; c >= 0; c--) {

            // check tile above where there is a row above
            if (r &&
                    (*boardLt[r-1][c] == 0 ||
                     *boardLt[r-1][c] == *boardLt[r][c]))
                return false;

            // check tile to left where there is a column to the left
            if (c &&
                    (*boardLt[r][c-1] == 0 ||
                     *boardLt[r][c-1] == *boardLt[r][c]))
                return false;
        }
    }

    return true; // no possible moves found
}

int main(int argc, char **argv)
{
    srand(time(NULL));         // seed random number
    signal(SIGINT, terminate); // set up signal to handle ctrl-c
    system("stty cbreak");     // read user input immediately
    initBoard();
    addRandom();

    for (;;) {
        printf("\e[1;1H\e[2J"); // clear screen
        addRandom();
        printBoard(boardLt);
        if (isFull()) break;
        if (win) quit(WIN);
        while(!move())
            ;
    }

    quit(LOSE); // game over
    cleanup();  // not reached
    return 0;
}
