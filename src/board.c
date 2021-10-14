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
    b->NextIsWhite = true;
    return b;
}

//Starts a new game
Board* newGameFromStart(){
    return newGameFromFen(START_FEN);
}

//Prints the board
void printBoard(Board *board){
    econio_clrscr();
    printf(" abcdefgh\n");
    for(int rank = 7; rank >= 0; rank--){
        printf("%d", rank+1);
        for(int file = 0; file < 8; file++){
            Piece p = board->Cell[rank][file];
            if(isValidPiece(p)){
                printf(getPieceFace(p));
            } else {
                printf(" ");
            }
        }
        printf("%d\n",rank+1);
    }
    printf(" abcdefgh\n");
}

Piece at(Board *board, Square square){
    return board->Cell[square.rank][square.file];
}

bool isFreeAt(Board *board, Square square){
    return !isValidPieceAt(board, square);
}

bool isBlackAt(Board *board, Square square){
    return isBlack(at(board, square));
}

bool isWhiteAt(Board *board, Square square){
    return isWhite(at(board, square));
}

bool isPawnAt(Board *board, Square square){
    return isPawn(at(board, square));
}

bool isRookAt(Board *board, Square square){
    return isRook(at(board, square));
}

bool isKnightAt(Board *board, Square square){
    return isKnight(at(board, square));
}

bool isBishopAt(Board *board, Square square){
    return isBishop(at(board, square));
}

bool isQueenAt(Board *board, Square square){
    return isQueen(at(board, square));
}

bool isKingAt(Board *board, Square square){
    return isKing(at(board, square));
}

bool isValidPieceAt(Board *board, Square square){
    return isValidPiece(at(board, square));
}

bool isOpponent(Board *board, Piece piece){
    if(board->NextIsWhite){
        return isBlack(piece);
    }
    return isWhite(piece);
}

bool isOpponentAt(Board *board, Square square){
    if(!isValidPieceAt(board, square)){
        return false;
    }
    //There is a piece, is it opponent's
    return isOpponent(board, at(board, square));
}