#ifndef CHESS_MOVES
#define CHESS_MOVES

#include <stdbool.h>

#include "board.h"
#include "piece.h"

typedef struct Move{
    Square from;
    Square to;
} Move;

//Moves a piece on a board, does NOT check if a move is valid
void movePiece(Board *board, Move move);

//Returnes true if the move is valid, taking mates into account
bool isValidMove(Board *board, Move move);

//Returnes the piece at the starting point of move
Piece pieceAtMoveFrom(Board *board, Move move);

//Returnes the piece at the ending point of move
Piece pieceAtMoveTo(Board *board, Move move);


//Returnes true if the move is valid for a pawn
bool checkPawnMove(Board *board, Move move);
//Returnes true if the move is valid for a rook
bool checkRookMove(Board *board, Move move);
//Returnes true if the move is valid for a knight
bool checkKnightMove(Board *board, Move move);
//Returnes true if the move is valid for a bishop
bool checkBishopMove(Board *board, Move move);
//Returnes true if the move is valid for a queen
bool checkQueenMove(Board *board, Move move);
//Returnes true if the move is valid for a king
bool checkKingMove(Board *board, Move move);

#endif