#ifndef CHESS_ALGEBRAIC
#define CHESS_ALGEBRAIC

#include "square.h"

Square algebraicToSquare(const char algebraic[2]);

//Saves a square's algebraic notation in out
void squareToAlgebraic(Square square, char out[2]);

#endif