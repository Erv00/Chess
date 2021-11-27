#include "check.h"

#include "square.h"
#include "algebraic.h"

#include <stdio.h>
#include <stdlib.h>
#include <debugmalloc.h>

CheckData isOpponentInCheck(Board *board){
    CheckData cd = {0};
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isValidPieceAt(board, s)){
                if(!isOpponentAt(board, s)){
                    Move m = {
                        .from = s,
                        .to = board->nextIsWhite ? board->blackKing : board->whiteKing
                    };

                    if(isValidMove(board, m, NULL, NULL, NULL)){
                        cd.inCheck = true;
                        cd.checkers[cd.numCheckers++] = s;
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
    CheckDataByColor checkD = isBoardInCheck(board);
    if(!checkD.white.inCheck && !checkD.black.inCheck)
        return false;
    
    Square checkedKing;
    CheckData activeCheck;
    //The two colors cannot be in check at the same time
    if(checkD.white.inCheck){
        checkedKing = board->whiteKing;
        activeCheck = checkD.white;
    } else {
        checkedKing = board->blackKing;
        activeCheck = checkD.black;
    }
    //Step out of check
    static int STEP_OFFSETS[] = {-1, -7, -8, -9, 1, 7, 8, 9};
    for(int i = 0;i < 8; i++){
        Move m;
        m.from = checkedKing;
        m.to = fromSquareID(toSquareID(m.from) + STEP_OFFSETS[i]);

        if(isValidMove(board, m, NULL, NULL, NULL))
            return false;
    }
    //Take checker
    //If more than one checker we cannot take both nor can we block it
    if(activeCheck.numCheckers > 1)
        return true;

    //Only one checker
    if(canAnyMoveTo(board, activeCheck.checkers[0])/* canBeTaken(board, activeCheck.checkers[0])*/)
        return false;

    //Block path
    //Cannot block pawn or knight
    if(isPawnAt(board, activeCheck.checkers[0]) || isKnightAt(board, activeCheck.checkers[0]))
        return true;
    
    if(nonEuclideanDistance(activeCheck.checkers[0], checkedKing) < 2)
        //Cannot block if directly next to king
        return true;
    return !canBeBlocked(board, activeCheck.checkers[0], checkedKing);
}

bool canBeBlocked(Board *board, Square what, Square target){
    do{
        what = stepToward(what, target);
        if(canAnyMoveTo(board, what))
            return true;
    }while(!isSame(what, target));
    return false;
}

bool canAnyMoveTo(Board *board, Square target){
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isSame(target, s))
                //Same as target, skip
                continue;
            
            Move m = {
                .from = s,
                .to = target
            };
            if(isValidMove(board, m, NULL, NULL, NULL))
                return true;
        }
    }
    return false;
}