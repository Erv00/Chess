#ifndef CHESS_CHECK
#define CHESS_CHECK

#include "board.h"
#include "moves.h"

typedef struct CheckData {
    bool inCheck;
    unsigned short numCheckers;
    Square checkers[2];
} CheckData;

typedef struct CheckDataByColor {
    //Contains data if white is in check
    CheckData white;
    //Contains data if black is in check
    CheckData black;
} CheckDataByColor;

//Sets the two pointed bools if the respective color is in check
//Any pointer may be NULL
//Only check for current color
CheckData isOpponentInCheck(Board *board);

//Returnes which colors will be in check if this move is played
CheckDataByColor willNextMoveBeCheck(Board *b, Move move);

//Returnes which if any color is in check
CheckDataByColor isBoardInCheck(Board *board);

bool isCheckmate(Board *board);

bool canBeBlocked(Board *board, Square what, Square target);

//Returnes true if the target square can be moved to by any opposing piece
bool canAnyMoveTo(Board *board, Square target);

#endif