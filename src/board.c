#include "board.h"
#include "piece.h"
#include "algebraic.h"
#include "moves.h"
#include "check.h"
#include "fen.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <debugmalloc.h>

Board* newGameFromStart(SDL_Renderer *renderer, int time){
    Board *board =  newGameFromFen(START_FEN, renderer);
    board->hasReplayData = true;
    board->whiteClock = newClock(time, renderer, true);
    board->blackClock = newClock(time, renderer, false);
    board->originalTime = time;
    return board;
}

void checkBoardStatus(Board *board){
    ReplayNode *node = board->replayData.last;
    //Check for checkmate
    CheckDataByColor cd = isBoardInCheck(board);
    if(cd.white.inCheck || cd.black.inCheck){
        //Node can be null if board has no replay data
        if(node != NULL)
            node->isCheck = true;
        if(isCheckmate(board)){
            board->checkmate = true;
            board->gameOver = true;
            board->whiteWon = !board->nextIsWhite;

            //Node can be null if board has no replay data
            if(node != NULL)
                node->isCheckmate = true;
        }
    }

    //50 move rule
    if(board->halfmoveClock >= 100){
        board->gameOver = true;
        board->draw = true;
        printf("DRAW\n");
    }
}

void saveWithoutMoves(const char *path, Board *board){
    FILE *saveFile = fopen(path, "w");
    if(saveFile == NULL){
        fprintf(stderr,"Failed to open savefile %s\n", path);
        return;
    }

    //Save position
    char *fen = saveAsFEN(board);
    //Save clock times
    fprintf(saveFile, "%s\n%d;%d\n", fen, board->whiteClock.secondsRemaining, board->blackClock.secondsRemaining);
    free(fen);
    //Close file
    fclose(saveFile);
}

Board* loadWithoutMoves(const char *path, SDL_Renderer *renderer){
    FILE *saveFile = fopen(path, "r");
    if(saveFile == NULL){
        fprintf(stderr, "Failed to open savefile %s\n", path);
        return NULL;
    }

    char fen[90]; //Bit extra for the longest possible fen string
    fgets(fen, 90, saveFile);

    //Get time
    int whiteTime, blackTime;
    int scanned = fscanf(saveFile, "%d;%d", &whiteTime, &blackTime);
    if(scanned != 2){
        //Not saved for continuing
        fprintf(stderr, "File not saved for continuing\n");
        return NULL;
    }

    Board *board = newGameFromFen(fen, renderer);
    board->whiteClock.secondsRemaining = whiteTime;
    board->blackClock.secondsRemaining = blackTime;
    
    //Update clock textures
    updateClockTexture(&board->whiteClock);
    updateClockTexture(&board->blackClock);

    fclose(saveFile);

    return board;
}

void destroyBoard(Board *board){
    deleteList(&board->replayData);
    free(board);
}

bool updateCorrectClock(Board *board){
    if(board->nextIsWhite)
        return clockTick(&board->whiteClock);
    return clockTick(&board->blackClock);
}

Piece *at(Board *board, Square square){
    return &board->cell[square.rank][square.file];
}

bool isFreeAt(Board *board, Square square){
    return !isValidPieceAt(board, square);
}

bool isWhiteAt(Board *board, Square square){
    return isWhite(*at(board, square));
}

bool isPawnAt(Board *board, Square square){
    return isPawn(*at(board, square));
}

bool isRookAt(Board *board, Square square){
    return isRook(*(at(board, square)));
}

bool isKnightAt(Board *board, Square square){
    return isKnight(*at(board, square));
}

bool isKingAt(Board *board, Square square){
    return isKing(*at(board, square));
}

bool isValidPieceAt(Board *board, Square square){
    return isValidPiece(*at(board, square));
}

bool isOpponent(Board *board, Piece piece){
    if(board->nextIsWhite){
        return isBlack(piece);
    }
    return isWhite(piece);
}

bool isOpponentAt(Board *board, Square square){
    if(!isValidPieceAt(board, square)){
        return false;
    }
    //There is a piece, is it opponent's
    return isOpponent(board, *at(board, square));
}

