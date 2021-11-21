/**
 * @file check.h
 * @brief Sakkhelyzet
 */
#ifndef CHESS_CHECK
#define CHESS_CHECK

#include "board.h"
#include "moves.h"

/**
 * @brief Egy sakk helyzetet ír le
 */
typedef struct CheckData {
    bool inCheck;               ///< Igaz, ha van sakk helyzet
    unsigned short numCheckers; ///< A sakkot adó bábuk száma, nem nagyobb mint 2
    Square checkers[2];         ///< A sakkot adó bábuk helyzetei. Nem érdemes dinamikusan kezelni, csak 2 lehet
} CheckData;

/**
 * @brief Színenkénti sakk helyzet leírása
 */
typedef struct CheckDataByColor {
    CheckData white;    ///< Fehér van sakkban
    CheckData black;    ///< Fekete van sakkban
} CheckDataByColor;

/**
 * @brief Meghatározza, hogy az ellenfél sakkban van-e
 * 
 * @param board A használt tábla
 * @return CheckData Információ a sakkról (ha van)
 */
CheckData isOpponentInCheck(Board *board);

/**
 * @brief Meghatározza, hogy az adott lépés után kik lesznek sakkban
 * 
 * @param board A tábla
 * @param move Az ellenőrizendő lépés
 * @return CheckDataByColor Színenkénti sakk(matt) információ
 */
CheckDataByColor willNextMoveBeCheck(Board *board, Move move);

/**
 * @brief Meghatározza, hogy a tábla jelenleg sakkban van-e
 * 
 * @param board Az ellenőrizendő tábla
 * @return CheckDataByColor Színenkénti sakk információ
 */
CheckDataByColor isBoardInCheck(Board *board);

/**
 * @brief Meghatározza, hogy sakkmat van-e
 * 
 * @param board Az ellenőrizendő tábla
 * @return true Ha sakkmatt van
 * @return false Ellenkező esetben
 */
bool isCheckmate(Board *board);

/**
 * @brief Meghatározza, hogy az adott sakkot adó bábu blokkolható-e
 * 
 * @param board Az ellenőrizendő tábla
 * @param what A blokkolandó bábu
 * @param target A sakkban tartott király
 * @return true Ha blokkolható a sakk
 * @return false Ellenkező esetben
 */
bool canBeBlocked(Board *board, Square what, Square target);

/**
 * @brief Meghatározza, hogy a most lépő szín bármelyik bábuja az adott mezőre tud-e lépni
 * 
 * @param board Az ellenőrizendő bábu
 * @param target A cél mező
 * @return true Ha van legális lépés
 * @return false Ellenkező esetben
 */
bool canAnyMoveTo(Board *board, Square target);

#endif