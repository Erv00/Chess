#ifndef CHESS_BOARD
#define CHESS_BOARD

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct Board {
    char Cell[8][8];
} Board;

//Loads a game state from a FEN string, and returnes the game
Board* newGameFromFen(const char* fenStr);

//Starts a new game
Board* newGameFromStart();

//Prints the board
void printBoard(Board *board);

#endif