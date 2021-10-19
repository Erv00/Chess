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

bool isBishop(Piece p){
    return (p & 7) == BISHOP;
}

bool isQueen(Piece p){
    return (p & 7) == QUEEN;
}

bool isKing(Piece p){
    return (p & 7) == KING;
}

bool isValidPiece(Piece p){
    return p & (COLOR_BLACK | COLOR_WHITE);
}

bool isSameColor(Piece p1, Piece p2){
    return (p1 & (COLOR_WHITE + COLOR_BLACK)) == (p2 & (COLOR_WHITE + COLOR_BLACK));
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