/**
 * @file loading.h
 * @brief Játékállás betöltésével kapcsolatos függvények
 */
#ifndef CHESS_LOADING
#define CHESS_LOADING

#include "board.h"
#include "analysis.h"

/**
 * @brief Egy lépést leíró sztringet rendel egy táblához
 * 
 * @param moveStr A lépés algebrai notációban
 * @param board A tábla amihez kapcsolódik a lépés
 * @return ReplayNode* A lépést leíró struktúra
 */
ReplayNode* parseMoveStr(const char *moveStr, Board *board);

/**
 * @brief Egy sáncolást leíró sztringet rendel egy táblához
 * 
 * @param moveStr A lépés algebrai notációban
 * @param node A lépést leíró struktúra
 * @param board A tábla amihez kapcsolódik a lépés
 */
void processCastlingStr(const char *moveStr, ReplayNode* node, Board *board);

/**
 * @brief Betölt egy játékmenetet analízishez
 * 
 * @param path A mentett fájl elérési útja
 * @return Board* A játék
 */
Board* loadMoves(const char *path);

#endif