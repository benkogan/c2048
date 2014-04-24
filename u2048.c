
/*
 * author: Ben Kogan <http://benkogan.com>
 *
 * 2048---game rules from <gabrielecirulli.github.io/2048/>.
 */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define GOAL 2048
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

int score = 0;
int win   = false;

static const int QUIT = -1,
                 LOSE =  0,
                 WIN  =  1;

void cleanUp() {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            free(boardLt[row][col]);
        }
    }
}

int quit(int op)
{
    char *msg = op==1 ? "YOU WIN!": op==0 ? "GAME OVER." : "QUIT";

    printf("\n\n%s\n", msg);
    cleanUp();
    exit(0);
}

void terminate(int signum) { quit(QUIT); }

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
            *tile = 0;

            // add same tile to equivalent location in  all directional boards
            boardLt[row][col]        = tile;
            boardRt[row][SIZE-1-col] = boardLt[row][col];
            boardUp[SIZE-1-col][row] = boardLt[row][col];
            boardDn[col][SIZE-1-row] = boardLt[row][col];
        }
    }
}

void printBoard(int *board[SIZE][SIZE])
{
    printf("\n2048\n\nSCORE: %d\n\n", score);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (*board[i][j])
                printf("%6d", *board[i][j]);
            else
                printf("%6s", ".");
        }
        printf("\n\n");
    }

    printf("\nMOVEMENT:\n   w\n a s d              ");
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

bool move(int *b[SIZE][SIZE])
{
    bool success = 0;  // true if something moves
    int marker   = -1; // marks a cell one past location of a previous merge

    for (int r = 0; r < SIZE; r++) { // rows
        for (int col = 1; col < SIZE; col++) { // don't need to slide first col

            // skip if current location has no tile
            if (!(*b[r][col])) continue;

            // advance to proper merge target in new column (nCol)
            int nCol = col;
            while (nCol && (!*b[r][nCol-1] || *b[r][nCol-1] == *b[r][col])) {
                nCol--;

                if (*b[r][nCol] == *b[r][col] || marker == nCol) break;
            }

            // merge tile with target tile
            if (nCol != col) {

                if (*b[r][nCol]) {
                    score += *b[r][nCol] += *b[r][col];
                    marker = nCol+1;
                    if (*b[r][nCol] == GOAL) win = true;
                } else {
                    *b[r][nCol] += *b[r][col];
                }

                *b[r][col] = 0;
                success = true;
            }
        }
        marker = -1; // reset for next row
    }

    return success;
}

int getMove()
{
    bool success = false;

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
    for (int row = SIZE-1; row >= 0; row--) {
        for (int col = SIZE-1; col >= 0; col--) {

            // check tile above where there is a row above
            if (row &&
                    (*boardLt[row-1][col] == 0 ||
                     *boardLt[row-1][col] == *boardLt[row][col]))
                return false;

            // check tile to left where there is a column to the left
            if (col &&
                    (*boardLt[row][col-1] == 0 ||
                     *boardLt[row][col-1] == *boardLt[row][col]))
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

    // infinite loop
    for (;;) {
        printf("\e[1;1H\e[2J"); // clear screen
        addRandom();
        printBoard(boardLt);
        if (isFull()) break;
        if (win) quit(WIN);
        while(!getMove())
            ;
    }

    // board is full with no more moves; game over
    quit(LOSE);

    cleanUp(); // not reached
    return 0;
}
