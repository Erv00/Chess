#include "algebraic.h"

Square algebraicToSquare(const char algebraic[2]){
    Square result;
    result.file = algebraic[0]-'a';
    result.rank = algebraic[1]-'1';
    return result;
}

void squareToAlgebraic(Square square, char out[2]){
    out[0] = square.file + 'a';
    out[1] = square.rank + '1';
}