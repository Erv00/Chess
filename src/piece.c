#include "piece.h"

#include <stdio.h>
#include <ctype.h>
#include "debugmalloc.h"

static const char* PIECE_FACES[] = {
    //WHITE
    "♙",
    "♖",
    "♘",
    "♗",
    "♕",
    "♔",

    //BLACK
    "♟︎",
    "♜",
    "♞",
    "♝",
    "♛",
    "♚"
};

bool isBlack(Piece p){
    return (p & 24) == COLOR_BLACK;
}

bool isWhite(Piece p){
    return (p & 24) == COLOR_WHITE;
}

bool isPawn(Piece p){
    return (p & 7) == PAWN;
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

const char* getPieceFace(Piece p){
    char offset = isBlack(p) ? 6 : 0;
    Piece pCode = p & 7;
    for(unsigned int i = 0;i < 7;i++){
        if(pCode == i){
            return PIECE_FACES[i+offset];
        }
    }
    fprintf(stderr, "No such piece: %d\n", p);
    return "X";
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
