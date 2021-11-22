#include "board.h"
#include "piece.h"
#include "algebraic.h"
#include "moves.h"
#include "check.h"
#include "fen.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <econio.h>
#include "debugmalloc.h"

//Starts a new game
Board* newGameFromStart(SDL_Renderer *renderer, int time){
    Board *board =  newGameFromFen(START_FEN, renderer);
    board->hasReplayData = true;
    board->whiteClock = newClock(time, renderer, true);
    board->blackClock = newClock(time, renderer, false);
    return board;
}

//Prints the board
void printBoard(Board *board){
    econio_clrscr();
    printf(" abcdefgh\n");
    for(int rank = 7; rank >= 0; rank--){
        printf("%d", rank+1);
        for(int file = 0; file < 8; file++){
            Piece p = board->cell[rank][file];
            if(isValidPiece(p)){
                printf(getPieceFace(p));
            } else {
                printf(" ");
            }
        }
        printf("%d\n",rank+1);
    }
    printf(" abcdefgh\n");

        econio_gotoxy(11,0);
    if(isValidSquare(board->enPassante)){
        char alg[3] = {0};
        squareToAlgebraic(board->enPassante, alg);
        printf("En passante: %s\n", alg);
    }else{
        printf("En passante: -\n");
    }

    //Check
    CheckDataByColor cd = isBoardInCheck(board);
    econio_gotoxy(11,1);
    printf("White is %sin check\n", cd.white.inCheck ? "" : "not ");
    econio_gotoxy(11,2);
    printf("Black is %sin check\n", cd.black.inCheck ? "" : "not ");
    econio_gotoxy(11,3);
    printf("Half moves %d, Full moves %d\n", board->halfmoveClock, board->fullmoveCounter);
    econio_gotoxy(11,4);
    printf("%s to move\n", board->nextIsWhite ? "White" : "Black");
    econio_gotoxy(11, 5);
    char cast[5] = {0};
    getCastlingString(board, cast);
    printf("Castling: %s\n", cast);
    if(board->checkmate)
            printf("CHECKMATE\n");

    econio_gotoxy(0,11);
}

void checkBoardStatus(Board *board){
    ReplayNode *node = board->replayData.last;
    //Check for checkmate
    CheckDataByColor cd = isBoardInCheck(board);
    if(cd.white.inCheck || cd.black.inCheck){
        node->isCheck = true;
        if(isCheckmate(board)){
            board->checkmate = true;
            board->gameOver = true;
            board->whiteWon = !board->nextIsWhite;

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
        fprintf(stderr,"Failed to open savefile %s. Aborting\n", path);
        exit(-1);
    }

    //Save position
    char *fen = saveAsFEN(board);
    fprintf(saveFile, "%s\n", fen);
    free(fen);
    //Close file
    fclose(saveFile);
}

Board* loadWithoutMoves(const char *path, SDL_Renderer *renderer){
    FILE *saveFile = fopen(path, "r");
    if(saveFile == NULL){
        fprintf(stderr, "Failed to open savefile %s. Aborting\n", path);
        exit(-1);
        abort();
    }

    char fen[90]; //Bit extra for the longest possible fen string
    fgets(fen, 90, saveFile);
    Board *board = newGameFromFen(fen, renderer);

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

