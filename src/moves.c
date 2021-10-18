#include "moves.h"

#include "piece.h"
#include "check.h"

#include <stdlib.h>
#include <stdio.h>

void movePiece(Board *board, Move move){
    if(isKingAt(board, move.from)){
        if(isWhiteAt(board, move.from)){
            board->whiteKing = move.to;
        }else{
            board->blackKing = move.to;
        }
    }
    board->cell[move.to.rank][move.to.file] = at(board, move.from);
    board->cell[move.from.rank][move.from.file] = 0;
}

void movePieceWithCheck(Board *board, Move move){
    Square enPassante = {-1, -1};
    Move rookMove = {.from.rank = -1};
    int newCastlingAvailability;
    if(!isValidMove(board, move, &enPassante, &rookMove, &newCastlingAvailability))
        return;
    
    //Check if hitting en passante
    if(isPawnAt(board, move.from) && isSame(move.to, board->enPassante)){
        //Take en passantable pawn
        if(board->nextIsWhite)
            //White moved, must take black pawn
            board->cell[move.to.rank-1][move.to.file] = 0;
        else
            //Black moved. must take white pawn
            board->cell[move.to.rank+1][move.to.file] = 0;
    }
    
    //En passante resets with every move
    board->enPassante = enPassante;

    //Castling
    if(rookMove.from.rank != -1){
        movePiece(board, rookMove);
        board->castlingAvailability = newCastlingAvailability;
    }
    movePiece(board, move);
}

bool isValidMove(Board *board, Move move, Square *enPassante, Move *rookMove, int *newCastlingAvailability){
    //Basic checks
    //Is in-bounds?
    if(!isValidSquare(move.from) || !isValidSquare(move.to)){
        return false;
    }

    //Is there a friendly piece to move?
    if(isFreeAt(board, move.from) || isOpponentAt(board, move.from)){
        return false;
    }

    //Is the destination occupied by a friendly piece?
    if(!isFreeAt(board, move.to) && !isOpponentAt(board, move.to)){
        return false;
    }

    bool valid = false;
    Piece toMove = at(board, move.from) & 7; //&7 to only select piece type, not color
    switch(toMove){
    case PAWN:
        valid = checkPawnMove(board, move, enPassante);
        break;
    case ROOK:
        valid = checkRookMove(board, move);
        break;
    case KNIGHT:
        valid = checkKnightMove(board, move);
        break;
    case BISHOP:
        valid = checkBishopMove(board, move);
        break;
    case QUEEN:
        valid = checkQueenMove(board, move);
        break;
    case KING:
        valid = checkKingMove(board, move, rookMove, newCastlingAvailability);
        break;
    //No need for default, as previous conditions ensure only the above are possible
    }

    if(!valid)
        return false;
    
    //Move may be valid, check if it will result in a check
    //Only check if not already checking if it's a king hit
    if(!isSame(move.to, board->whiteKing) && !isSame(move.to, board->blackKing)){
    bool whiteCheck = false, blackCheck = false;
    willNextMoveBeCheck(board, move, &whiteCheck, &blackCheck);
        if(board->nextIsWhite && whiteCheck)
            //White cannot move to put themselves in check
            return false;
        if(!board->nextIsWhite && blackCheck)
            //Black cannot move to put themselves in check
            return false;
    }
    return true;
}

bool checkPawnMove(Board *board, Move move, Square *enPassante){
    int from = toSquareID(move.from);
    int to = toSquareID(move.to);

    int diff = to-from;

    //Check for backward moves
    if(board->nextIsWhite){
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
        bool isValid = isFreeAt(board, move.to) && isFreeAt(board, middle);

        //Save en passantable square
        if(isValid && enPassante != NULL)
            *enPassante = middle;
        
        return isValid;
    } else {
        return isOpponentAt(board, move.to) || isSame(move.to, board->enPassante);
    }

}

bool checkRookMove(Board *board, Move move){
    //Check if directionally OK
    if(move.from.rank != move.to.rank && move.from.file != move.to.file){
        return false;
    }

    //Check if path is free
    Square current = stepToward(move.from, move.to);
    while(!isSame(current, move.to)){
        if(!isFreeAt(board, current)){
            return false;
        }
        current = stepToward(current, move.to);
    }

    return true;
}

bool checkKnightMove(Board *board, Move move){
    static const int KNIGHT_OFFSETS[] = {6, 10, 15, 17};
    int diff = abs(toSquareID(move.to) - toSquareID(move.from));

    for(int i = 0; i < 4; i++){
        if(diff == KNIGHT_OFFSETS[i]){
            return true;
        }
    }

    return false;
}

bool checkBishopMove(Board *board, Move move){
    //Check if diagnal move
    //This works beacuse of maths, see documentation
    int diff = toSquareID(move.from) - toSquareID(move.to);
    if(diff % 7 != 0 && diff % 9 != 0){
        return false;
    }

    //Check if path is free
    Square current = stepToward(move.from, move.to);
    while(!isSame(current, move.to)){
        if(!isFreeAt(board, current)){
            return false;
        }
        current = stepToward(current, move.to);
    }

    return true;
}

bool checkQueenMove(Board *board, Move move){
    return checkBishopMove(board, move) || checkRookMove(board, move);
}

bool checkKingMove(Board *board, Move move, Move *castling, int *newCastlingAvailability){
    int dist = nonEuclideanDistance(move.from, move.to);
    if(dist == 1)
        return true;
    
    if(dist != 2)
        return false;
    
    //Check for castling
    if(move.from.rank != move.to.rank)
        return false;
    
    int kingside = move.from.file < move.to.file ? 0 : 1;
    int white = board->nextIsWhite ? 0 : 2;

    int castlingID = white+kingside;

    bool isValid = board->castlingAvailability & (1 << castlingID);

    if(isValid && castling != NULL){
        castling->from.rank = move.to.rank;
        castling->to.rank = move.to.rank;
        //Select correct rook
        castling->from.file = kingside ? 0 : 7;
        //Select file
        castling->to.file = !kingside ? move.to.file-1 : move.to.file+1;
        if(newCastlingAvailability != NULL)
            *newCastlingAvailability = board->castlingAvailability ^ (1 << castlingID);
    }

    return isValid;
}