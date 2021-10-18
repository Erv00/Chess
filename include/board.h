#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <stdbool.h>

#include "square.h"
#include "piece.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct Move Move;

typedef struct Board {
    char cell[8][8];
    bool nextIsWhite;
    char castlingAvailability;
    Square enPassante;
    int halfmoveClock;
    int fullmoveCounter;
    Square whiteKing;
    Square blackKing;
} Board;

//Loads a game state from a FEN string, and returnes the game
Board* newGameFromFen(const char* fenStr);

//Starts a new game
Board* newGameFromStart();

//Prints the board
void printBoard(Board *board);

Piece at(Board *board, Square square);

//Returnes true if the specified cell is free
bool isFreeAt(Board *board, Square square);


bool isBlackAt(Board *board, Square square);
bool isWhiteAt(Board *board, Square square);
bool isPawnAt(Board *board, Square square);
bool isRookAt(Board *board, Square square);
bool isKnightAt(Board *board, Square square);
bool isBishopAt(Board *board, Square square);
bool isQueenAt(Board *board, Square square);
bool isKingAt(Board *board, Square square);
bool isValidPieceAt(Board *board, Square square);
//Returnes true if there is an opponent piece at square
bool isOpponentAt(Board *board, Square square);
//Returnes true if the piece is the opponents
bool isOpponent(Board *board, Piece piece);

#endif