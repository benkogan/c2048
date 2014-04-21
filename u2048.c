
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#define MAX_STRING_LEN 1000
#define SIZE  4
int board[SIZE][SIZE];
//char out[MAX_STRING_LEN];
char *out;                                                                      //TODO: USE A BUFFER DUDE

// IDEAS:
//      - implement as an arch linux kernel module?
//      - themed? can pass a param, or even a text file with equivalencies;
//        dynamically adjust board size

void initBoard()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

void printBoard()
{
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {

            char *out = NULL;

            if (board[i][j])
                printf(" %d ", board[i][j]);
                // sprintf(out, "%d", board[i][j]);
            else
                printf(" %s ", "~");
                // out = "~";

            //out = board[i][j] ? (char) board[i][j] : "~";
            // printf(" %s ", out);
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
    } while (board[i][j] != 0);

    int random = rand()%2; // 0 or 1
    printf("\nrandom: %d\n", 2 * random + 2);
    printf("board i j: %d, %d\n", i, j);
    board[i][j] = 2 * random + 2; // 2 or 4
}

int moveLeft()
{
    bool success = 0; // true if something moves

    for (int col = 0; col < SIZE; col++) {
        for (int row = 1; row < SIZE; row++) { // don't need to slide first row
            
            // skip if no tile here                                             // TODO: necessary? checked in next step, but no action taken there
            if (!board[row][col])
                continue;

            // advance newCol to border or first non-0
            int newCol = col;
            while (newCol && !board[row][newCol])
                newCol--;

            // move tile to newCol
            if (!board[row][newCol] || board[row][newCol] == board[row][col]) {
                board[row][newCol] += board[row][col];
                board[row][col] = 0;
                success = true;                                                 //TODO: add a stopping element to prevent double mergers; either in previous loc or in newCol-1
            }
        }
    }

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
    printf("QUIT! Bye bye.\n");
    exit(signum);
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    signal(SIGINT, quit); // set up signal to handle ctrl-c

    initBoard();
    addRandom();
    addRandom();

    printBoard();

    while(!getMove())
        ;

    printBoard();                                                               //TODO: bash `clear` before print

    return 0;
}


