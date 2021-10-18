#ifndef CHESS_CHECK
#define CHESS_CHECK

#include "board.h"
#include "moves.h"

//Sets the two pointed bools if the respective color is in check
//Any pointer may be NULL
void isInCheck(Board *board, bool *isWhiteInCheck, bool *isBlackInCheck);

void willNextMoveBeCheck(Board *b, Move move, bool *isWhiteInCheck, bool *isBlackInCheck);

bool isCheckmate(Board *board);

#endif