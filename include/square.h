#ifndef CHESS_SQUARE
#define CHESS_SQUARE

#include <stdbool.h>

typedef struct Square{
    int rank, file;
} Square;

//Returnes true if the square is in bounds
bool isValidSquare(Square square);

bool isSame(Square a, Square b);

//Returnes the numeric ID of a square [0,64) from a1 to h8
int toSquareID(Square square);

int ranksToTop(Square square);
int ranksToBottom(Square square);

int filesToLeft(Square square);
int filesToRight(Square square);

//Returnes a square so that it's one step towards to
Square stepStraight(Square from, Square to);
#endif