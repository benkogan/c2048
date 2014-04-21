
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_STRING_LEN 1000
#define SIZE 4
int board[SIZE][SIZE];
//char out[MAX_STRING_LEN];
char *out; //TODO: USE A BUFFER DUDE

// IDEAS:
//      - implement as an arch linux kernel module?
//      - themed? can pass a param, or even a text file with equivalencies; dynamically adjust board size

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

void getMove()
{
    int TRUE = 1,
        FALSE = 0,
        success; // a boolean

    while (TRUE) {
        char move = getchar();
        switch(move) {
            case 119:      // 'w' key
                success = moveUp();
                break;
            case 97:       // 'a' key
                success = moveLeft();
                break;
            case 115:       // 's' key
                success = moveDown();
                break;
            case 100:       // 'd' key
                success = moveRight();
                break;
            default:
                success = FALSE;
        }
    }
}

int isFull()
{
    int full = 1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            


            

        }
    }


}

//TODO: MAYBE THERE's A MORE EFFICIENT WAY THAN BELOW; e.g., store an array of where there are 0's
void addRandom()
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

    //TODO: bash `clear` before print
    printBoard();

    return 0;
}


