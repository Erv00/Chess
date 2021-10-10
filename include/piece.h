#ifndef CHESS_PIECE
#define CHESS_PIECE

#include <stdbool.h>

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

typedef enum Piece {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING,

    //Color
    COLOR_WHITE = 8,
    COLOR_BLACK = 16
} Piece;

bool isBlack(Piece p);
bool isWhite(Piece p);
bool isPawn(Piece p);
bool isRook(Piece p);
bool isKnight(Piece p);
bool isBishop(Piece p);
bool isQueen(Piece p);
bool isKing(Piece p);
bool isValidPiece(Piece p);

//Returnes the string to print for a piece
const char* getPieceFace(Piece p);

#endif