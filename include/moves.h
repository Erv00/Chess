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

//Moves a piece on a board, checks if move is valid, also updates the board's state (enPassante, castlingAvailability)
void movePieceWithCheck(Board *board, Move move);

//Returnes true if the move is valid, taking mates into account
//enPassante will contain the en passantable square if not NULL
//rookMove will contain a valid move to preform castling if not NULL
//rookMove should not be NULL if newCastlingAvailability is not NULL
bool isValidMove(Board *board, Move move, Square *enPassante, Move *rookMove, int *newCastlingAvailability);

//Returnes the piece at the starting point of move
Piece pieceAtMoveFrom(Board *board, Move move);

//Returnes the piece at the ending point of move
Piece pieceAtMoveTo(Board *board, Move move);


//Returnes true if the move is valid for a pawn
//If this creates an en passantable square, save it
bool checkPawnMove(Board *board, Move move, Square *enPassante);
//Returnes true if the move is valid for a rook
bool checkRookMove(Board *board, Move move, int *newCastlingAwailability);
//Returnes true if the move is valid for a knight
bool checkKnightMove(Board *board, Move move);
//Returnes true if the move is valid for a bishop
bool checkBishopMove(Board *board, Move move);
//Returnes true if the move is valid for a queen
bool checkQueenMove(Board *board, Move move);
//Returnes true if the move is valid for a king
//If it's a castling move, also return the move to move the rook
bool checkKingMove(Board *board, Move move, Move *castlingMove, int *newCastlingAvailability);

bool canCastle(Board *board, bool white, bool kingside);
#endif