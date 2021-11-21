/**
 * @file promotion.h
 * @brief Átalakulás
 */
#ifndef CHESS_PROMOTION
#define CHESS_PROMOTION

#include <SDL2/SDL.h>
#include "board.h"
#include "square.h"
#include "piece.h"

/**
 * @brief Megjeleníti a választó menüt
 * 
 * @param board A tábla amelyen a választás történik
 */
void drawPicker(Board *board);

/**
 * @brief Lehetőséget nyújt egy bábu kiválasztására, és visszadja
 * 
 * @param board A tábla amelyen a választás történik
 * @return Piece A választott bábu, érvénytelen ha választást megszakítják
 */
Piece getChoice(Board *board);
/**
 * @brief Ellenőrzi, hogy egérrel választottunk-e bábut
 * 
 * @param color Az a szín, amit ki kell rajzolni
 * @return Piece A választott bábu
 */
Piece checkMouseSelect(Piece color);

/**
 * @brief Ellenőrzi, hogy billentyűzettel választottunk-e bábut
 * 
 * @param color A szín, amit ki kell rajzolni
 * @param e A gombnyomás event
 * @return Piece A választott bábu
 */
Piece checkKeyboardSelect(Piece color, SDL_Event *e);

#endif