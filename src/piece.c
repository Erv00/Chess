#include "piece.h"

#include <stdio.h>
#include <ctype.h>
#include <debugmalloc.h>

bool isBlack(Piece p){
    return (p & 24) == COLOR_BLACK;
}

bool isWhite(Piece p){
    return (p & 24) == COLOR_WHITE;
}

bool isPawn(Piece p){
    return (p & 7) == PAWN;
}

bool isRook(Piece p){
    return (p & 7) == ROOK;
}

bool isKnight(Piece p){
    return (p & 7) == KNIGHT;
}

bool isKing(Piece p){
    return (p & 7) == KING;
}

bool isValidPiece(Piece p){
    return p & (COLOR_BLACK | COLOR_WHITE);
}

static const char PIECE_CHAR[] = {'P', 'R', 'N', 'B', 'Q', 'K'};

char getPieceChar(Piece p){
    Piece pCode = p & 7;
    for(unsigned int i = 0; i < 7; i++){
        if(pCode == i)
            return PIECE_CHAR[i];
    }
    fprintf(stderr, "No such piece: %d\n", p);
    return 'X';
}

Piece getPieceFromChar(const char ch){
    for(Piece p = PAWN; p <= KING; p++){
        if((PIECE_CHAR[p] | 32) == (ch | 32)){
            if(isupper(ch))
                return p | COLOR_WHITE;
            else
                return p | COLOR_BLACK;
        }
    }
    fprintf(stderr, "No such piece %c\n", ch);
    //Return invalid piece
    return 0;
}
