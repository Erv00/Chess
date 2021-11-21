#include "analysis.h"

#include <stdlib.h>

#include "board.h"
#include "algebraic.h"
#include "graphics.h"
#include "debugmalloc.h"
#include "fen.h"
#include "loading.h"

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
        node->previous = list->last;
        list->last = node;
    }

    list->length += 1;
}

void appendPointerToList(ReplayList *list, ReplayNode *node){
    if(list->first == NULL){
        //List is empty
        list->first = node;
        list->last = node;
    } else {
        //Node is not empty, append after last
        list->last->next = node;
        node->previous = list->last;
        list->last = node;
    }

    list->length += 1;
}

void deleteList(ReplayList *list){
    ReplayNode *head = list->first;
    while(head != NULL){
        ReplayNode *next = head->next;
        if(head->texture != NULL)
            SDL_DestroyTexture(head->texture);
        free(head);
        head = next;
    }
    list->first = NULL;
    list->last = NULL;
    list->length = 0;
}

void freeListAfter(ReplayList *list,ReplayNode *node){
    if(node == NULL)
        return;
    ReplayNode *head = node->next;
    while(head != NULL){
        ReplayNode *next = head->next;
        free(head);
        head = next;
        list->length -= 1;
    }
    node->next = NULL;
}
    
enum MoveUniqueness isMoveUnique(Board *board, Move move){
    //If rook either file or rank may be needed, not both
    //If else both may be required
    if(isPawnAt(board, move.from))
        return UNIQUE;

    enum MoveUniqueness ret = UNIQUE;

    Move capture = move;

    Piece checkedPiece = *at(board, move.from);

    if(!isKnight(checkedPiece)){
        //Check in file, by changeing rank
        for(int rank = 0; rank < 8; rank++){
            capture.from.rank = rank;
            if(isSame(capture.from, move.from))
                //Don't count the piece we're checking
                continue;

            if(*at(board, capture.from) == checkedPiece){
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

            if(*at(board, capture.from) == checkedPiece){
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
                else if(knightMove.from.rank == move.from.rank)
                    ret &= ~RANK_UNIQUE;
                else if(ret == UNIQUE)
                    //
                    ret &= ~RANK_UNIQUE;
            }
        }
    }

    return ret;
}

void nodeToString(ReplayNode *node, char *out){
    if(node->isCastling){
        //Castling
        memcpy(out, "0-0", 3);
        out += 3;
        if(!node->castlingIsKingSide){
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
}

void createNodeTexture(ReplayNode *node, SDL_Renderer *renderer){
    //Don't recreate if node already has it created
    if(node->texture != NULL)
        return;

    char notation[8] = {0};
    nodeToString(node, notation);
    node->texture = stringToTexture(renderer, notation, NULL, NULL);
    if(node->texture == NULL)
        exit(-1);
}

void renderReplay(ReplayList list, SDL_Renderer *renderer){
    SDL_Rect dst = {
        .x = 8*45,
        .y = 0,
    };

    //Only print last 15 or so full moves
    int stepNumber = 1;
    int idx = 0;
    bool first = true;
    ReplayNode *node = list.first;
    if(list.length > 15*2){
        //More than we can display, advance
        for(int length = list.length; length > 15*2; length -= 2){ 
            node = node->next->next;
            ++stepNumber;
            idx += 2;
        }
    }


    while(node != NULL && idx < list.length){
        //If first, print step number
        if(first){
            char str[7] = {0}; //6 characters are enough, the longest possible chess game is ~9000 moves
            sprintf(str, "%d. ", stepNumber);
            SDL_Texture *tex = stringToTexture(renderer, str, &dst.w, &dst.h);
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            dst.x += dst.w;
            SDL_DestroyTexture(tex);
        }
        
        if(node->texture == NULL)
            createNodeTexture(node, renderer);

        SDL_QueryTexture(node->texture, NULL, NULL, &dst.w, &dst.h);

        SDL_RenderCopy(renderer, node->texture, NULL, &dst);

        if(first){
            //Printed first, go to next column
            dst.x = 10*45;
        }else{
            //Printed second, go to next row
            dst.y += dst.h;
            dst.x = 8*45;
            stepNumber++;
        }

        first = !first;

        node = node->next;
        idx++;
    }
}

void saveWithMoves(const char *path, Board *board){
    FILE *saveFile = fopen(path, "w");
    if(saveFile == NULL)
        //HANDLE: TODO
        return;

    //Save position
    char *fen = saveAsFEN(board);
    fprintf(saveFile, "%s\n", fen);
    free(fen);

    //If no moves have been made, skip
    if(board->replayData.length != 0){
        //Save moves
        char nodeStr[8] = {0};

        ReplayNode *node = board->replayData.first;
        while(node != NULL){
            nodeToString(node, nodeStr);
            fprintf(saveFile, "%s\n", nodeStr);
            
            node = node->next;
        }
    }

    //Close file
    fclose(saveFile);
}

void undoMove(Board *board, ReplayNode *node){
    //Move piece back
    Move rev = {.from = node->move.to, .to = node->move.from};
    movePiece(board, rev);
    //Put captured piece back
    if(node->isCapture)
        *at(board, rev.from) = node->pieceCaptured;
    //If promotion, demote
    if(node->isPromotion){
        //Demote promoted piece
        *at(board, rev.to) = PAWN | (node->isWhiteMove ? COLOR_WHITE : COLOR_BLACK);
    }
    //Move rook back if needed
    if(node->isCastling){
        Move rookMove = {.from = node->rookMove.to, .to = node->rookMove.from};
        movePiece(board, rookMove);
    }
}

void redoMove(Board *board, ReplayNode *node){
    movePiece(board, node->move);
    if(node->isCastling){
        movePiece(board, node->rookMove);
    }

    //If promotion, promote
    if(node->isPromotion)
        *at(board, node->move.to) = node->promotionChoice;
}

bool handleAnalysisKeypress(Board *board, SDL_Event ev){
    ReplayNode *next;
    switch(ev.key.keysym.sym){
        case SDLK_LEFT:
            //Step back
            if(board->replayData.length != 0){
                //Undo step
                undoMove(board, board->replayData.last);
                
                //Update replay list
                board->replayData.last = board->replayData.last->previous;
                board->replayData.length -= 1;
                
                //Flip next turn
                board->nextIsWhite = !board->nextIsWhite;
            }
            break;
        case SDLK_RIGHT:
            //Step forward
            if(board->replayData.last == NULL)
                //Step to first move
                next = board->replayData.first;
            else
                //Step to next move
                next = board->replayData.last->next;

            if(next != NULL){
                //Redo move
                redoMove(board, next);

                //Update replay list
                board->replayData.last = next;
                board->replayData.length += 1;

                //Flip next turn
                board->nextIsWhite = !board->nextIsWhite;
            }
            break;
        case SDLK_q:
            return true;
    }
    return false;
}