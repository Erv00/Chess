#include "board.h"
#include "piece.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <econio.h>

void processFENPieceData(Board *board, const char *fenStr){
    //Pieces
    int rank = 7, file = 0;
    while(*fenStr != ' '){
        if(*fenStr == '/'){
            //Skip to next rank
            rank--;
            file = 0;
        }else if(isdigit(*fenStr)){
            //Skip ahead
            file += *fenStr - '0';
        }else{
            //Piece data
            Piece piece = isupper(*fenStr) ? COLOR_WHITE : COLOR_BLACK;
            switch(*fenStr | 32){ // OR 32 is to only deal with lowercase characters
            case 'p':
                piece |= PAWN;
                break;
            case 'r':
                piece |= ROOK;
                break;
            case 'n':
                piece |= KNIGHT;
                break;
            case 'b':
                piece |= BISHOP;
                break;
            case 'q':
                piece |= QUEEN;
                break;
            case 'k':
                piece |= KING;
                break;
            default:
                printf("Unknown character in FEN string: %c\n", *fenStr);
            }
    
            board->Cell[rank][file] = piece;
            file++;
        }
        fenStr++;
    }
}

Board* newGameFromFen(const char* fenStr){
    Board *b = calloc(1,sizeof(Board));
    processFENPieceData(b, fenStr);
    return b;
}

//Starts a new game
Board* newGameFromStart(){
    return newGameFromFen(START_FEN);
}

//Prints the board
void printBoard(Board *board){
    //econio_textcolor(COL_BLACK);
    //econio_textbackground(COL_WHITE);
    printf(" abcdefgh\n");
    for(int rank = 7; rank >= 0; rank--){
        printf("%d", rank+1);
        for(int file = 0; file < 8; file++){
            Piece p = board->Cell[rank][file];
            if(isValidPiece(p))
                printf(getPieceFace(p));
        }
        printf("\n");
    }
    printf(" abcdefgh\n");
}