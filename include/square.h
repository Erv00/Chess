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
Square fromSquareID(int squareID);

int ranksToTop(Square square);
int ranksToBottom(Square square);

int filesToLeft(Square square);
int filesToRight(Square square);

//Returnes a square so that it's one step towards to
//Should only be used when in a straight line or in a diagonal
Square stepToward(Square from, Square to);

//Returnes the non-euclidean distance of a and b, such that
//the return value is = max(deltaX, deltaY) 
int nonEuclideanDistance(Square a, Square b);
#endif