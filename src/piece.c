#include "piece.h"

#include <stdio.h>

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
    return p & COLOR_BLACK;
}

bool isWhite(Piece p){
    return p & COLOR_WHITE;
}

bool isPawn(Piece p){
    return p & PAWN;
}

bool isRook(Piece p){
    return p & ROOK;
}

bool isKnight(Piece p){
    return p & KNIGHT;
}

bool isBishop(Piece p){
    return p & BISHOP;
}

bool isQueen(Piece p){
    return p & QUEEN;
}

bool isKing(Piece p){
    return p & KING;
}

bool isValidPiece(Piece p){
    return p & (COLOR_BLACK | COLOR_WHITE);
}

const char* getPieceFace(Piece p){
    char offset = isBlack(p) ? 6 : 0;
    Piece pCode = p & 7;
    for(unsigned int i = 0;i < 6;i++){
        if(pCode == i){
            return PIECE_FACES[i+offset];
        }
    }
    fprintf(stderr, "No such piece: %d\n", p);
    return "X";
}