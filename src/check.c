#include "check.h"

#include "square.h"

#include <stdlib.h>

void isInCheck(Board *board, bool *whiteInCheck, bool *blackInCheck){
    for(int rank = 0; rank < 8; rank++){
        for(int file = 0; file < 8; file++){
            Square s = {.rank = rank, .file = file};
            if(isSame(s, board->whiteKing) || isSame(s, board->blackKing)){
                //No need to check kings
                continue;
            }
            if(isValidPieceAt(board, s)){
                if(isWhiteAt(board, s) && blackInCheck != NULL){
                    //Check if white can hit black
                    Move m = {
                        .from = s,
                        .to = board->blackKing
                    };

                    *blackInCheck |= isValidMove(board, m, NULL, NULL, NULL);                    
                } else if(isBlackAt(board, s) && whiteInCheck != NULL){
                    Move m = {
                        .from = s,
                        .to = board->whiteKing
                    };

                    *whiteInCheck |= isValidMove(board, m, NULL, NULL, NULL);
                }
            }
        }
    }
}

void willNextMoveBeCheck(Board *b, Move move, bool *isWhiteInCheck, bool *isBlackInCheck){
    Board copy = *b; //Copy structure
    movePiece(&copy, move);
    copy.nextIsWhite = !copy.nextIsWhite;
    isInCheck(&copy, isWhiteInCheck, isBlackInCheck);
}

bool isCheckmate(Board *board){
    //Step out of check
    static STEP_OFFSETS[] = {-1, -7, -8, -9, 1, 7, 8, 9};
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