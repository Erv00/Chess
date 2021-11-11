#include "square.h"

#include <stdlib.h>

#include "debugmalloc.h"

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

Square fromSquareID(int squareID){
    Square sq = {
        .rank = squareID / 8,
        .file = squareID % 8
    };
    return sq;
}

Square stepToward(Square from, Square to){
    if(isSame(from, to))
        return from;
    
    //One will always be zero
    int rankDir = to.rank - from.rank;
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

int nonEuclideanDistance(Square a, Square b){
    int rankDist = abs(a.rank - b.rank);
    int fileDist = abs(a.file - b.file);
    if(rankDist >= fileDist)
        return rankDist;
    return fileDist;
    
}