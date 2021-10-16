#ifndef CHESS_ALGEBRAIC
#define CHESS_ALGEBRAIC

#include "square.h"

Square algebraicToSquare(const char algebraic[2]);

void squareToAlgebraic(Square square, char out[2]);

#endif