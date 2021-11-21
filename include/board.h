/**
 * @file board.h
 * @brief Sakktábla
 */
#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "square.h"
#include "piece.h"
#include "mouse.h"
#include "analysis.h"
#include "clock.h"

/**
 * @brief A hivatalos kezdés FEN sztringje
 */
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

/**
 * @brief Egy sakktábla reprezentálása
 */
typedef struct Board {
    Piece cell[8][8];           ///< A 8x8-as tábla
    bool nextIsWhite;           ///< Igaz, ha a következő lépés fehéré
    char castlingAvailability;  ///< A sáncolási lehetőségek bitfalagként 8 = fekete Q 4 = fekete K 2 = fehér Q 1 = fehér K
    Square enPassante;          ///< Az en passante lépéssel ráléphető négyzet
    int halfmoveClock;          ///< Az eddig megtett félkörök száma az 50 lépéses szabály értelmében
    int fullmoveCounter;        ///< Az eddig megtett körök száma
    Square whiteKing;           ///< A fehér király helyzete
    Square blackKing;           ///< A fekete király helyzete
    bool checkmate;             ///< Igaz, ha sakkmatt

    Clock whiteClock;           ///< Fehér sakkórája
    Clock blackClock;           ///< Fekete sakkórája

    MouseState mouseState;      ///< Az egér állapota

    SDL_Renderer *renderer;     ///< A táblához kapcsolt renderer

    //False if loaded without moves
    bool hasReplayData;         ///< Igaz, ha tudjuk hogyan jutott a játék ebbe az állásba
    ReplayList replayData;      ///< Láncolt lista, hogy hogyan jutottunk el az eddigi játékállásig

    bool quit;                  ///< Igaz, ha ki szeretnénk lépni
    bool gameOver;              ///< Igaz, ha a játéknak vége
    bool draw;                  ///< Igaz, ha döntetlen a játék
    bool whiteWon;              ///< Igaz, ha fehér nyert
} Board;

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

/**
 * @brief Ellenőrzi, és frissíti a tábla állapotát (50 lépés, sakk(matt) 
 * @param board Az ellenőrizendő tábla
 */
void checkBoardStatus(Board *board);

/**
 * @brief Elmenti a táblát FEN notációban
 * 
 * @param path 
 * @param board 
 */
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

/**
 * @brief Visszad egy mutatót a megadott mezőn lévő bábura
 * 
 * @param board A tábla
 * @param square A megadott mező
 * @return Piece* A bábura mutató pointer
 */
Piece* at(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mező szabad-e
 * 
 * @param board A használt tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mező szabad
 * @return false Ha a mező nem szabad
 */
bool isFreeAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn fehér áll-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn fehár áll
 * @return false Ellenkező esetben
 */
bool isWhiteAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn gyalog áll-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn gyalog áll
 * @return false Ellenkező esetben
 */
bool isPawnAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn huszár áll-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn huszár áll
 * @return false Ellenkező esetben
 */
bool isKnightAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn király áll-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn király áll
 * @return false Ellenkező esetben
 */
bool isKingAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn áll-e bábu
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn áll bábu
 * @return false Ellenkező esetben
 */
bool isValidPieceAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott mezőn ellenfél áll-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param square Az ellenőrizendő mező
 * @return true Ha a mezőn ellenfél áll
 * @return false Ellenkező esetben
 */
bool isOpponentAt(Board *board, Square square);

/**
 * @brief Meghatározza, hogy az adott bábu ellenfél-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param piece Az ellenőrizendő bábu
 * @return true Ha a bábu ellenfél
 * @return false Ellenkező esetben
 */
bool isOpponent(Board *board, Piece piece);

#endif
