
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX_STRING_LEN 1000
#define SIZE 4

/*
 * boardLt is the canonical board; all other boards reflect LEFT as their
 * respective direction. To move a tile in any direction, it will be moved
 * LEFT on its respective directional board.
 *
 * For example, "up" on the canonical board is LEFT on boardUp.
 */
int *(boardLt[SIZE][SIZE]); //  left moves (the "canonical" board)
int *(boardRt[SIZE][SIZE]); // right moves
int *(boardUp[SIZE][SIZE]); //    up moves
int *(boardDn[SIZE][SIZE]); //  down moves

void initBoard()
{
    for (int row = 0; row < SIZE; row++) { // row for canonical board
        for (int col = 0; col < SIZE; col++) { // col for canonical board

            // create a new, empty tile
            int *tile = (int *)malloc(sizeof(int *));
            if (tile == NULL) {
                perror("malloc returned NULL");
                exit(1);
            }

            // add same tile to equivalent location in  all directional boards
            boardLt[row][col]        = tile;
            boardRt[row][SIZE-1-col] = boardLt[row][col];
            boardUp[SIZE-1-col][row] = boardLt[row][col];
            boardDn[col][SIZE-1-row] = boardLt[row][col];
        }
    }
}

// TODO: reimpliment using buffer?
void printBoard(int *board[4][4])
{
    printf("\n2048\n\nSCORE: \n\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (*board[i][j])
                printf("%6d", *board[i][j]);
            else
                printf("%6s", ".");
        }
        printf("\n\n");
    }

    printf("\nMOVEMENT:\n   w\n a s d\n\n");

}

void addRandom()
{
    int i, j;
    do {
        i = rand()%4; // random number from 0 to 3
        j = rand()%4;
    } while (*boardLt[i][j] != 0);

    int random = rand()%2; // 0 or 1
    *boardLt[i][j] = 2 * random + 2; // 2 or 4
}

bool move(int *board[4][4])
{
    bool success = 0; // true if something moves

    for (int row = 0; row < SIZE; row++) {
        for (int col = 1; col < SIZE; col++) { // don't need to slide first row

            // end if current location has no tile
            if (!(*board[row][col]))
                continue;

            //TODO: simplify this block; 3am code is bad code lol
            int newCol = col;
            for (;;) {
                if ( !newCol || (*board[row][newCol-1] != 0 &&
                *board[row][newCol-1] != *board[row][col]) )
                    break;

                newCol--;

                if (*board[row][newCol] == *board[row][col]) // equals current
                    break;
            }


            // move tile to newCol
//          if (!(*board[row][newCol]) || *board[row][newCol] == *board[row][col]) {
            if (newCol != col) {
                *board[row][newCol] += *board[row][col];
                *board[row][col] = 0;
                //TODO: add a stopping element to prevent double mergers;
                //      either in previous loc or in newCol-1
                success = true;
            }
        }
    }

    return success;
}

int getMove()
{
    bool success;

    char direction = getchar();
    switch(direction) {
        case 119:      // 'w' key; up
            success = move(boardUp);
            break;
        case 97:       // 'a' key; left
            success = move(boardLt);
            break;
        case 115:       // 's' key; down
            success = move(boardDn);
            break;
        case 100:       // 'd' key; right
            success = move(boardRt);
            break;
        default:
            success = false;
    }

    return success;
}

void quit(int signum)
{
    printf("\n\nTERMINATED! Bye bye.\n");
    exit(signum);
}

int main(int argc, char **argv)
{
    srand(time(NULL));     // seed random number
    signal(SIGINT, quit);  // set up signal to handle ctrl-c
    system("stty cbreak"); // read user input immediately

    initBoard();
    addRandom(); // first run has two random tiles; here's the first

    // infinite loop
    for (;;) {
        printf("\e[1;1H\e[2J"); // clear screen
        addRandom();
        printBoard(boardLt);
        while(!getMove())
            ;
    }

    return 0;
}
