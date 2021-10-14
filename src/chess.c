#include <stdio.h>

#include "board.h"
#include "moves.h"

int main(void){
    Move m;
    char ff,fr,tf,tr;

    Board *b = newGameFromStart();
    printBoard(b);
    while(true){
        scanf("%c%c %c%c%*c", &ff, &fr, &tf, &tr);
        if(ff == 'q'){
            return 0;
        }
        m.from.file = ff-'a';
        m.to.file = tf-'a';
        m.from.rank = fr-'1';
        m.to.rank = tr-'1';
        if(isValidMove(b, m)){
            movePiece(b, m);
            printBoard(b);
        }else{
            printf("Invalid move\n");
        }
    }
}