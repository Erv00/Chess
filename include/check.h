#ifndef CHESS_CHECK
#define CHESS_CHECK

#include "board.h"
#include "moves.h"

typedef struct CheckData {
    bool inCheck;
    Square checker;
} CheckData;

typedef struct CheckDataByColor {
    CheckData white, black;
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

#endif