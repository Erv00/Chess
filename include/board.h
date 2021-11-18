#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "square.h"
#include "piece.h"
#include "mouse.h"
#include "analysis.h"
#include "clock.h"

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct Board {
    char cell[8][8];
    bool nextIsWhite;
    char castlingAvailability;
    Square enPassante;
    int halfmoveClock;
    int fullmoveCounter;
    Square whiteKing;
    Square blackKing;
    bool checkmate;

    Clock whiteClock, blackClock;

    MouseState mouseState;
    //Renderer associated with the board, may be null if nut using graphics
    SDL_Renderer *renderer;

    //False if loaded without moves
    bool hasReplayData;
    ReplayList replayData;

    bool quit;
    bool gameOver;
    bool draw;
    bool whiteWon;
} Board;

//Loads a game state from a FEN string, and returnes the game
Board* newGameFromFen(const char* fenStr, SDL_Renderer *renderer);

//Saves the castling string in cast, does NOT append \0
//Returnes the length of the string
int getCastlingString(Board *board, char cast[4]);

/**
 * @brief Új játékot kezd az alapállásból
 * 
 * @param renderer A használt renderer
 * @param time Egy játékosra jutó idő, másodpercben
 * @return Board* A játéktábla
 */
Board* newGameFromStart(SDL_Renderer *renderer, int time);

//Prints the board
void printBoard(Board *board);

//Returnes the game's FEN string
char *saveAsFEN(Board *board);

//Check the board's status: Check[mate], fifty-move rule
void checkBoardStatus(Board *board);

void saveWithoutMoves(const char *path, Board *board);

/**
 * @brief Betölt egy játékállást
 * 
 * @param path A betöltendő fájl elérési útja
 * @param renderer A tábla renderere
 * @return Board* Egy tábla amely a megfelelő állásban van
 */
Board* loadWithoutMoves(const char *path, SDL_Renderer *renderer);

/**
 * @brief Felszabadítja a táblát, és hozzá tartozó replayData-t
 * 
 * @param board A felszabadítandó tábla
 */
void destroyBoard(Board *board);

/**
 * @brief Lépteti a megfelelő órát
 * 
 * @param board A tábla aminek az óráját lépteti
 * @return true Ha az adoot óra lejárt
 * @return false Ellenkező esetben
 */
bool updateCorrectClock(Board *board);

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

bool isSameColorAt(Board *board, Square s1, Square s2);

#endif
