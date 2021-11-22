#include "moves.h"

#include "piece.h"
#include "check.h"
#include "promotion.h"
#include "analysis.h"

#include <stdlib.h>
#include <stdio.h>
#include "debugmalloc.h"

void movePiece(Board *board, Move move){
    if(isKingAt(board, move.from)){
        if(isWhiteAt(board, move.from)){
            board->whiteKing = move.to;
        }else{
            board->blackKing = move.to;
        }
    }
    *at(board, move.to) = *at(board, move.from);
    *at(board, move.from) = 0;
}

void movePieceWithCheck(Board *board, Move move){
    Square enPassante = {-1, -1};
    Move rookMove = {.from.rank = -1};
    int newCastlingAvailability = -1;
    if(!isValidMove(board, move, &enPassante, &rookMove, &newCastlingAvailability)){
        return;
    }

    //Move is valid
    ReplayNode rNode = {.isWhiteMove = board->nextIsWhite, .move = move, .movedPiece = *at(board, move.from), .moveUniqueness = isMoveUnique(board, move)};

    if(isPawnAt(board, move.from) && (move.to.rank == 0 || move.to.rank == 7)){
        //Promote pawn
        Piece choice = getChoice(board);
        if(!isValidPiece(choice))
            return;
        
        *at(board, move.from) = choice;
        rNode.isPromotion = true;
        rNode.promotionChoice = choice;
    }

    //Set node
    if(!isFreeAt(board, move.to)){
        rNode.isCapture = true;
        rNode.pieceCaptured = *at(board, move.to);
    }

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

    //Update board state
    //50 move rule
    if(isPawnAt(board, move.from) || !isFreeAt(board, move.to))
        board->halfmoveClock = 0;
    else
        board->halfmoveClock++;
    
    //Move counter
    if(!board->nextIsWhite)
        board->fullmoveCounter++;

    if(newCastlingAvailability != -1){
        //Castling availability changed
        rNode.castlingData.oldCastlingAvailability = board->castlingAvailability;
        rNode.castlingData.newCastlingAvailability = newCastlingAvailability;

        board->castlingAvailability = newCastlingAvailability;
    }

    //Castling
    if(rookMove.from.rank != -1){
        movePiece(board, rookMove);
        rNode.isCastling = true;
        if(rookMove.from.file == 7)
            rNode.castlingData.isKingside = true;
    }
    movePiece(board, move);

    //Push to list, last element will be modified by checkBoardStatus to reflect check and checkmate data
    appendToList(&board->replayData, rNode);

    board->nextIsWhite = !board->nextIsWhite;
    checkBoardStatus(board);
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
    Piece toMove = *at(board, move.from) & 7; //&7 to only select piece type, not color
    switch(toMove){
    case PAWN:
        valid = checkPawnMove(board, move, enPassante);
        break;
    case ROOK:
        valid = checkRookMove(board, move, newCastlingAvailability);
        break;
    case KNIGHT:
        valid = checkKnightMove(move);
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
    default:
        //No need for default, as previous conditions ensure only the above are possible, but compiler warnings are treated as errors
        break;
    }

    if(!valid)
        return false;
    
    //Move may be valid, check if it will result in a check
    //Only check if not already checking if it's a king hit
    if(!isSame(move.to, board->whiteKing) && !isSame(move.to, board->blackKing)){
        CheckDataByColor cd = willNextMoveBeCheck(board, move);
        if(board->nextIsWhite && cd.white.inCheck)
            //White cannot move to put themselves in check
            return false;
        if(!board->nextIsWhite && cd.black.inCheck)
            //Black cannot move to put themselves in check
            return false;
    }
    return true;
}

bool checkPawnMove(Board *board, Move move, Square *enPassante){
    int from = toSquareID(move.from);
    int to = toSquareID(move.to);

    int diff = to-from;

    //Check for board edge
    if(abs(move.from.file - move.to.file) > 1)
        return false;

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
        //Takes piece
        return isOpponentAt(board, move.to) || isSame(move.to, board->enPassante);
    }

}

bool checkRookMove(Board *board, Move move, int *newCastlingAvailability){
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

    //Remove castling
    if(newCastlingAvailability != NULL){
        if(move.from.rank == 0 || move.from.rank == 7){
            if(move.from.file == 0 || move.from.file == 7){
                int mask = 1;
                if(!board->nextIsWhite)
                    mask = mask << 2;
                //If queenside
                if(move.from.file == 0)
                    mask = mask << 1;

                *newCastlingAvailability = board->castlingAvailability & ~mask;
            }
        }
    }

    return true;
}

bool checkKnightMove(Move move){
    int rankDiff = abs(move.from.rank - move.to.rank);
    int fileDiff = abs(move.from.file - move.to.file);
    if(rankDiff == 1 && fileDiff == 2)
        return true;
    if(rankDiff == 2 && fileDiff == 1)
        return true;

    return false;
}

bool checkBishopMove(Board *board, Move move){
    //Check if diagnal move
    int rankDiff = move.from.rank-move.to.rank;
    int fileDiff = move.from.file-move.to.file;
    if(abs(rankDiff) != abs(fileDiff)){
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
    return checkBishopMove(board, move) || checkRookMove(board, move, NULL);
}

bool checkKingMove(Board *board, Move move, Move *castling, int *newCastlingAvailability){
    int dist = nonEuclideanDistance(move.from, move.to);
    if(dist == 1){
        //Normal move, remove castling availability
        if(newCastlingAvailability != NULL){
            if(board->nextIsWhite)
                *newCastlingAvailability = board->castlingAvailability & (4+8);
            else
                *newCastlingAvailability = board->castlingAvailability & (1+2);
        }
        return true;
    }
    
    if(dist != 2)
        return false;
    
    //Check for castling
    if(move.from.rank != move.to.rank)
        return false;
    
    bool kingside = move.to.file == 6;

    int mask = 1;
    if(!board->nextIsWhite)
        mask = mask << 2;
    //Is queenside
    if(move.to.file == 2)
        mask = mask << 1;

    bool isValid = board->castlingAvailability & mask;

    //Can only castle if square in between is not under threat
    Square middle = stepToward(move.from, move.to);
    board->nextIsWhite = !board->nextIsWhite;
    if(canAnyMoveTo(board, middle)){
        //Is under threat
        board->nextIsWhite = !board->nextIsWhite;
        return false;
    }
    board->nextIsWhite = !board->nextIsWhite;

    if(isValid && castling != NULL){
        castling->from.rank = move.to.rank;
        castling->to.rank = move.to.rank;
        //Select correct rook
        castling->from.file = kingside ? 7 : 0;
        //Select file
        castling->to.file = kingside ? move.to.file-1 : move.to.file+1;
        if(newCastlingAvailability != NULL){
            if(board->nextIsWhite)
                *newCastlingAvailability = board->castlingAvailability & (4+8);
            else
                *newCastlingAvailability = board->castlingAvailability & (1+2);
        }
    }

    return isValid;
}

char getCastlingAvailabilityAfterMove(Board *board, Move move){
    if(isKingAt(board, move.from)){
        //King moves, no more castling
        char mask = 3;
        if(isWhiteAt(board, move.from))
            mask = mask << 2;
        
        return board->castlingAvailability & mask;
    }else if(isRookAt(board, move.from)){
        //If not moving from starting position, ignore
        //Rank check
        if(move.from.rank != 0 && move.from.rank != 7)
            return board->castlingAvailability;
        
        //File check
        if(move.from.file != 0 && move.from.file != 7)
            return board->castlingAvailability;
        
        //Remove just the rook's castling
        char mask = 1;
        if(!isWhiteAt(board, move.from))
            mask = mask << 2;

        if(move.from.file == 0)
            //Long castling
            mask = mask << 1;

        return board->castlingAvailability & ~mask;
    }

    return board->castlingAvailability;
}

bool canCastle(Board *board, bool white, bool kingside){
    int mask = 1;
    if(!white)
        mask = mask << 2;
    if(!kingside)
        mask = mask << 1;
    
    return (board->castlingAvailability & mask) != 0;
}

Square findPawnMovableTo(Square to, Piece moved, Board *board){
    Move move = {
        .from = to,
        .to = to
    };
    int dir = -1;
    if(isBlack(moved))
        dir = 1;

    //Normal move
    move.from.rank += dir;
    if(isValidSquare(move.from) && *at(board, move.from) == moved)
        if(isValidMove(board, move, NULL, NULL, NULL))
            return move.from;
    //Capture
    move.from.file += dir;
    if(isValidSquare(move.from) && *at(board, move.from) == moved)
        if(isValidMove(board, move, NULL, NULL, NULL))
            return move.from;

    move.from.file -= 2*dir;
    if(isValidSquare(move.from) && *at(board, move.from) == moved)
        if(isValidMove(board, move, NULL, NULL, NULL))
            return move.from;

    //2 square move
    move.from.rank += dir;
    move.from.file += dir;
    if(isValidSquare(move.from) && *at(board, move.from) == moved)
        if(isValidMove(board, move, NULL, NULL, NULL))
            return move.from;

    //Should not happen
    fprintf(stderr, "File corrupted\n");
    exit(-1);
}

Square findRookMovableTo(Square to, Piece moved, Board *board){
    Move move = {to, to};

    //Check in rank
    for(int file = 0; file < 8; file++){
        move.from.file = file;
        if(*at(board, move.from) == moved)
            if(isValidMove(board, move, NULL, NULL, NULL))
                return move.from;
    }

    move.from.file = to.file;

    //Check in file
    for(int rank = 0; rank < 8; rank++){
        move.from.rank = rank;
        if(*at(board, move.from) == moved)
            if(isValidMove(board, move, NULL, NULL, NULL))
                return move.from;
    }

    //Should only happen if checking queen
    Square ret = {-1, -1};
    return ret;
}

Square findKnightMovableTo(Square to, Piece moved, Board *board){
    int rankDiff = 2;
    int fileDiff = 1;

    Move move = {to, to};

    for(int i=0; i<4; i++){ //There are 4 cases
        move.from.rank += rankDiff;
        move.from.file += fileDiff;
        if(*at(board, move.from) == moved)
            if(isValidMove(board, move, NULL, NULL, NULL))
                return move.from;
        
        move.from = to;
        if(fileDiff == -1)
            rankDiff *= -1;
        fileDiff *= -1;
    }

    rankDiff = 1;
    fileDiff = 2;

    for(int i=0; i<4; i++){ //There are 4 cases
        move.from.rank += rankDiff;
        move.from.file += fileDiff;
        if(*at(board, move.from) == moved)
            if(isValidMove(board, move, NULL, NULL, NULL))
                return move.from;
        
        move.from = to;
        if(fileDiff == -2)
            rankDiff *= -1;
        fileDiff *= -1;
    }

    //Should not happen
    fprintf(stderr, "File corrupted\n");
    exit(-1);
}

Square findBishopMovableTo(Square to, Piece moved, Board *board){
    int rankDiff = 1;
    int fileDiff = 1;

    Move move = {to, to};
    //There are 4 cases
    for(int i=0; i<4;i++){
        do{
            move.from.rank += rankDiff;
            move.from.file += fileDiff;
            if(isValidSquare(move.from))
                if(*at(board, move.from) == moved)
                    if(isValidMove(board, move, NULL, NULL, NULL))
                        return move.from;
        }while(isValidSquare(move.from));
        move.from = to;
        if(rankDiff == -1)
            fileDiff = -1;
        rankDiff *= -1;
    }

    //Should only happen if checking queen
    Square ret = {-1, -1};
    return ret;
}

Square findQueenMovableTo(Square to, Piece moved, Board *board){
    Square ret = findRookMovableTo(to, moved, board);
    if(isValidSquare(ret))
        return ret;
    
    return findBishopMovableTo(to, moved, board);
}