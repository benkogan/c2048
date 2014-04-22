
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

// IDEAS:
//  - themed? input txt file with equivalencies; dynamically adjust board to fit

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

void printBoardNew(int *board[4][4], char *direction)
{
    printf("PRINT BOARD NEW: %s\n", direction);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (*board[i][j])
                printf(" %d ", *board[i][j]);
            else
                printf(" %s ", "~");
        }
        printf("\n");
    }


}

void printBoard()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            if (*boardLt[i][j])
                printf(" %d ", *boardLt[i][j]);
            else
                printf(" %s ", "~");
        }
        printf("\n");
    }
}

void printBoardRt()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (*boardRt[i][j])
                printf(" %d ", *boardRt[i][j]);
            else
                printf(" %s ", ".");
        }
        printf("\n");
    }
}

void addRandom()                                                                //TODO: maybe more efficient way; e.g., store an array of where there are 0's
{
    int i, j;
    do {
        i = rand()%4; // random number from 0 to 3
        j = rand()%4;
    } while (*boardLt[i][j] != 0);

    int random = rand()%2; // 0 or 1
    *boardLt[i][j] = 2 * random + 2; // 2 or 4
}

int moveLeft()
{
    bool success = 0; // true if something moves

    for (int row = 0; row < SIZE; row++) {
        for (int col = 1; col < SIZE; col++) { // don't need to slide first row
            printf("\nFor row[%d] col[%d]", row, col);

            // skip if no tile here                                             // TODO: necessary? checked in next step, but no action taken there
            if (!(*boardLt[row][col]))
                continue;
            printf(" (noskip)");

            // advance newCol to border or 1 before another tile, ie. a pos. int  // TODO: keep an eye on here for bugs
            int newCol = col;
            while (newCol && !(*boardLt[row][newCol-1]))
                newCol--;
            printf(" (newCol:%d)", newCol);

            // move tile to newCol
            if (!(*boardLt[row][newCol]) || *boardLt[row][newCol] == *boardLt[row][col]) {
                *boardLt[row][newCol] += *boardLt[row][col];
                *boardLt[row][col] = 0;
                success = true;                                                 //TODO: add a stopping element to prevent double mergers; either in previous loc or in newCol-1
            }
        }
    }
    printf("\n\n");

    return success;
}

int getMove()
{
    bool success;

    char move = getchar();
    switch(move) {
        case 119:      // 'w' key
//            success = moveUp();
            break;
        case 97:       // 'a' key
            success = moveLeft();
            break;
        case 115:       // 's' key
//            success = moveDown();
            break;
        case 100:       // 'd' key
//            success = moveRight();
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
    srand(time(NULL));
    signal(SIGINT, quit); // set up signal to handle ctrl-c

//  initBoardRt();
//  printBoardRt();

    initBoard();
    addRandom();
    addRandom();

    printBoardNew(boardLt, "left");
    printBoardNew(boardRt, "right");
    printBoardNew(boardUp, "up");
    printBoardNew(boardDn, "down");

    while(!getMove())
        ;

//  printBoard();                                                               //TODO: bash `clear` before print
//  printBoardRt();

    printBoardNew(boardLt, "left");
    printBoardNew(boardRt, "right");
    printBoardNew(boardUp, "up");
    printBoardNew(boardDn, "down");

    return 0;
}
