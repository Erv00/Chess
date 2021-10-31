#include "analysis.h"

#include <stdlib.h>

#include "board.h"
#include "algebraic.h"

void appendToList(ReplayList *list, ReplayNode data){
    ReplayNode *node = malloc(sizeof(ReplayNode));
    if(node == NULL){
        fprintf(stderr, "Failed to allocate memory for DLL\n");
        exit(-1);
    }
    //Copy data into dynamic memory
    *node = data;

    if(list->first == NULL){
        //List is empty
        list->first = node;
        list->last = node;
    } else {
        //Node is not empty, append after last
        list->last->next = node;
        list->last = node;
    }
}

enum MoveUniqueness isMoveUnique(Board *board, Move move){
    //If rook either file or rank may be needed, not both
    //If else both may be required

    enum MoveUniqueness ret = UNIQUE;

    Move capture = move;

    Piece checkedPiece = at(board, move.from);

    if(!isKnight(checkedPiece)){
        //Check in file, by changeing rank
        for(int rank = 0; rank < 8; rank++){
            capture.from.rank = rank;
            if(isSame(capture.from, move.from))
                //Don't count the piece we're checking
                continue;

            if(at(board, capture.from) == checkedPiece){
                //Same piece type, good candidate
                if(isValidMove(board, capture, NULL, NULL, NULL)){
                    //Piece in the same file can take target
                    ret ^= FILE_UNIQUE;
                    break;
                }
            }
        }

        //Chek in rank, by changeing file
        capture.from = move.from;
        for(int file = 0; file < 8; file++){
            capture.from.file = file;
            if(isSame(capture.from, move.from))
                //Don't count the piece we're checking
                continue;

            if(at(board, capture.from) == checkedPiece){
                //Same piece type, good candidate
                if(isValidMove(board, capture, NULL, NULL, NULL)){
                    ret ^= RANK_UNIQUE;
                    //We can safely return, no more checks
                    return ret;
                }
            }
        }
    }else{
        //Is a kight, need special check
        static const int KNIGHT_OFFSETS[] = {-17, -15, -10, -6, 6, 10, 15, 17};

        Move knightMove = {
            .to = move.to
        };

        for(int i=0; i<8; i++){
            knightMove.from = fromSquareID(KNIGHT_OFFSETS[i] + toSquareID(move.to));
            if(isSame(knightMove.from, move.from))
                continue;

            if(isKnightAt(board, knightMove.from) && isValidMove(board, knightMove, NULL, NULL, NULL)){
                if(knightMove.from.file == move.from.file)
                    ret &= ~FILE_UNIQUE;
                //else if(knightMove.from.rank == move.from.rank && (ret & FILE_UNIQUE) != FILE_UNIQUE)
                //    ret &= ~RANK_UNIQUE;
                else if(knightMove.from.rank == move.from.rank)
                    ret &= ~RANK_UNIQUE;
            }
        }
    }

    return ret;
}

int nodeToString(ReplayNode *node, char *out){
    char *orig = out;
    if(node->isCastling){
        //Castling
        memcpy(out, "0-0", 3);
        out += 3;
        if(node->castlingIsKingSide){
            memcpy(out, "-0", 2);
            out += 2;
        }
    } else {
        //Not castling
        //Print piece if not pawn
        if(!isPawn(node->movedPiece))
            *out++ = getPieceChar(node->movedPiece);
        
        //Print unique identifier
        if(node->moveUniqueness != UNIQUE){
            //Move is not unique
            if((node->moveUniqueness & RANK_UNIQUE) != RANK_UNIQUE){
                //Rank not unique, disambiguate by printing file
                *out++ = node->move.from.file + 'a';
            }
            if((node->moveUniqueness & FILE_UNIQUE) != FILE_UNIQUE){
                //File not unique, disambiguate by printing rank
                *out++ = node->move.from.rank + '1';
            }
        }

        //If capture, print x
        if(node->isCapture){
            //Special case: pawn, print file
            if(isPawn(node->movedPiece))
                *out++ = node->move.from.file + 'a';
            *out++ = 'x';
        }
        
        //Print target square
        squareToAlgebraic(node->move.to, out);
        out += 2;

        //Is promotion
        if(node->isPromotion)
            *out++ = getPieceChar(node->promotionChoice);
        
    }

    //Status markers
    if(node->isCheckmate)
        *out++ = '#';
    else if(node->isCheck)
        *out++= '+';

    *out = '\0';
    return out - orig + 1;
}