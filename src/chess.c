#include <stdio.h>

#include "board.h"
#include "moves.h"
#include "algebraic.h"

int main(void){
    Move m;
    char moveData[4];

    Board *b = newGameFromStart();
    printBoard(b);
    while(true){
        scanf("%2s %2s", moveData, moveData+2);
        while (getchar() != '\n');
        if(*moveData == 'q'){
            return 0;
        }
        m.from = algebraicToSquare(moveData);
        m.to = algebraicToSquare(moveData+2);
        if(isValidMove(b, m, NULL, NULL, NULL)){
            movePieceWithCheck(b, m);
            printBoard(b);
            b->nextIsWhite = !b->nextIsWhite;
        }else{
            printf("Invalid move\n");
        }
    }
}