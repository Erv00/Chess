#include "square.h"

#include <math.h>

bool isValidSquare(Square square){
    return  (0 <= square.rank && square.rank < 8) &&
            (0 <= square.file && square.file < 8);
}

bool isSame(Square a, Square b){
    return a.rank == b.rank && a.file == b.file;
}

int toSquareID(Square square){
    return square.rank*8+square.file;
}

Square stepStraight(Square from, Square to){
    if(isSame(from, to))
        return from;
    
    int rankDir = (to.rank - from.rank);
    int fileDir = to.file - from.file;
    
    //Normalize if needed
    if(rankDir != 0)
        rankDir = rankDir > 0 ? 1 : -1;
    if(fileDir != 0)
        fileDir = fileDir > 0 ? 1 : -1;

    Square ret;
    ret.rank = from.rank + rankDir;
    ret.file = from.file + fileDir;
    return ret;
}