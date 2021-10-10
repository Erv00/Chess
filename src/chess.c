#include <stdio.h>

#include "board.h"

int main(void){
    Board *b = newGameFromStart();
    printBoard(b);
}