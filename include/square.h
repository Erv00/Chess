/**
 * @file square.h
 * @brief Sakktáblán egy mező leírása
 */
#ifndef CHESS_SQUARE
#define CHESS_SQUARE

#include <stdbool.h>

/**
 * @brief Egy mező
 */
typedef struct Square{
    int rank;   //!< Sor
    int file;   //!< Oszlop
} Square;

/**
 * @brief Meghatározza, hogy az adott mező valós-e
 * @param square A vizsgálandó mező
 * @return true Ha a mező valós
 * @return false Ellenkező esetben
 */
bool isValidSquare(Square square);

/**
 * @brief Meghatározza, hogy két mező egyenlő-e
 * @param a Az egyik mező
 * @param b A másik mező
 * @return true Ha a mezők megegyeznek
 * @return false Ellenkező esetben
 */
bool isSame(Square a, Square b);

/**
 * @brief Meghatározza egy mező numerikus azonosítóját.
 * Az azonosító az a1-es (bal alsó) mezőtől kezdődik 0-val
 * @param square A vizsgálandó mező
 * @return int A mező numerikus azonosítója
 */
int toSquareID(Square square);

/**
 * @brief A toSquareID inverze
 * @see toSquareID
 * @param squareID Egy mező numerikus azonosítója a [0,64) intervallumon
 * @return Square A megadott mező
 */
Square fromSquareID(int squareID);

/**
 * @brief Megahtározza az egy lépéssel közellebi lépést from-tól, to-hoz
 * A két mezőnek egy sorban, oszlopban, vagy átlóban kell lenniük
 * @param from A kiinduló mező
 * @param to A célmező
 * @return Square Az egy lépéssel közelebbi mező
 */
Square stepToward(Square from, Square to);

/**
 * @brief Meghatározza két mező nem euklédeszi távolságát
 * A nem euklédeszi távolság d' = max(sorEltérés, oszopEltérés)
 * @param a Az egyik mező
 * @param b A másik mező
 * @return int A nem euklédeszi távolság
 */
int nonEuclideanDistance(Square a, Square b);
#endif