/**
 * @file algebraic.h
 * @brief Algebrai notáció
 */
#ifndef CHESS_ALGEBRAIC
#define CHESS_ALGEBRAIC

#include "square.h"

/**
 * @brief Egy mező algebrai leírásából meghatározza a megfelelő mezőt
 * 
 * @param algebraic A mező algebrai leírása 2 karakterrel
 * @return Square A meghatározott mező
 */
Square algebraicToSquare(const char algebraic[2]);

/**
 * @brief Egy mezőnek adja meg a algebrai leírást 2 karakterben
 * 
 * @param square A mező
 * @param out Ide íródik a mező algebrai alakja
 */
void squareToAlgebraic(Square square, char out[2]);

#endif