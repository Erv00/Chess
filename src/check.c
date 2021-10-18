#include "check.h"

#include "square.h"
#include "algebraic.h"

#include <stdio.h>
#include <stdlib.h>

CheckData isOpponentInCheck(Board *board){
    CheckData cd = {0};
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isSame(s, board->whiteKing) || isSame(s, board->blackKing)){
                //No need to check kings
                continue;
            }
            if(isValidPieceAt(board, s)){
                if(!isOpponentAt(board, s)){
                    Move m = {
                        .from = s,
                        .to = board->nextIsWhite ? board->blackKing : board->whiteKing
                    };

                    if(isValidMove(board, m, NULL, NULL, NULL)){
                        cd.inCheck = true;
                        cd.checker = s;
                        return cd;
                    }

                }
            }
        }
    }
    return cd;
}

CheckDataByColor willNextMoveBeCheck(Board *b, Move move){
    Board copy = *b; //Copy structure
    movePiece(&copy, move);
    return isBoardInCheck(&copy);
}

CheckDataByColor isBoardInCheck(Board *board){
    bool savedColor = board->nextIsWhite;
    CheckDataByColor result;
    board->nextIsWhite = true;
    result.black = isOpponentInCheck(board);
    board->nextIsWhite = false;
    result.white = isOpponentInCheck(board);
    board->nextIsWhite = savedColor;
    return result;
}

bool isCheckmate(Board *board){
    //Step out of check
    static int STEP_OFFSETS[] = {-1, -7, -8, -9, 1, 7, 8, 9};
    for(int i = 0;i < 8; i++){
        Move m;
        m.from = board->nextIsWhite ? board->whiteKing : board->blackKing;
        m.to = fromSquareID(toSquareID(m.from) + STEP_OFFSETS[i]);

        if(isValidMove(board, m, NULL, NULL, NULL))
            return false;
    }
    //Take checker
    //Block path
    return true;
}