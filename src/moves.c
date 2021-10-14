#include "moves.h"

#include "piece.h"

void movePiece(Board *board, Move move){
    board->Cell[move.to.rank][move.to.file] = at(board, move.from);
    board->Cell[move.from.rank][move.from.file] = 0;
}

bool isValidMove(Board *board, Move move){
    //Basic checks
    //Is in-bounds?
    if(!isValidSquare(move.from) || !isValidSquare(move.to)){
        return false;
    }

    //Is there a piece there?
    if(isFreeAt(board, move.from)){
        return false;
    }

    //Is the destination occupied by a friendly piece?
    if(!isFreeAt(board, move.to)){
        //Space is NOT free
        if(!isOpponentAt(board, move.to)){
            return false;
        }
    }

    Piece toMove = at(board, move.from) & 7; //&7 to only select piece type, not color
    switch(toMove){
    case PAWN:
        return checkPawnMove(board, move);
    case ROOK:
        return checkRookMove(board, move);
    case KNIGHT:
        //return checkKnightMove(board, move);
    case BISHOP:
        //return checkBishopMove(board, move);
    case QUEEN:
        //return checkQueenMove(board, move);
    case KING:
        //return checkKingMove(board, move);
    }

    return true;
}

bool checkPawnMove(Board *board, Move move){
    int from = toSquareID(move.from);
    int to = toSquareID(move.to);

    int diff = to-from;

    //Check for backward moves
    if(board->NextIsWhite){
        if(to <= from){
            return false;
        }
    } else {
        if(to >= from){
            return false;
        }
        diff *= -1;
    }

    if(diff < 7){//7 because of capturing left
        //Can only be achived by a sideways move, invalid
        return false;
    }
    if(diff > 2*8){
        //Bigger than a 2 square advance, invalid
        return false;
    }
    if(diff < 2*8 && diff > 8+1){
        //Not a 2 square advance but bigger than a right capture
        return false;
    }

    if(diff == 8){
        //Advances normally
        return isFreeAt(board, move.to);
    } else if(diff == 2*8){
        //Advances two ranks, check if first step
        if(move.from.rank != 1 && move.from.rank != 6){
            return false;
        }
        Square middle;
        middle.rank = (move.to.rank + move.from.rank) / 2; //Sum is always integer because this is a 2 square advance
        middle.file = move.to.file; //File doesn't change
        return isFreeAt(board, move.to) && isFreeAt(board, middle);
    } else {
        return isOpponentAt(board, move.to);
    }

}

bool checkRookMove(Board *board, Move move){
    //Check if directionally OK
    if(move.from.rank != move.to.rank && move.from.file != move.to.file){
        return false;
    }

    //Check if path is free
    Square current = stepStraight(move.from, move.to);
    while(!isSame(current, move.to)){
        if(!isFreeAt(board, current)){
            return false;
        }
        current = stepStraight(current, move.to);
    }

    return true;
}

bool checkKnightMove(Board *board, Move move);
bool checkBishopMove(Board *board, Move move);
bool checkQueenMove(Board *board, Move move);
bool checkKingMove(Board *board, Move move);