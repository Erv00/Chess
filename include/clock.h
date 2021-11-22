/**
 * @file clock.h
 * @brief Sakk óra
 */
#ifndef CHESS_CLOCK
#define CHESS_CLOCK

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct Board Board;

/**
 * @brief Óra
 */
typedef struct Clock {
    int secondsRemaining;   ///< Hátralévő másodpercek
    SDL_Texture *texture;   ///< Az óra textúrája
    bool whitesClock;       ///< Fehér órája?

    SDL_Renderer *renderer; ///< Font rendereléshez használt renderer
} Clock;

/**
 * @brief Új órát hoz létre a megadott idővel
 * 
 * @param seconds A kezdeti érték
 * @param renderer Az óra renderere
 * @param whites Igaz ha az óra fehéré
 * @return Clock Az új óra
 */
Clock newClock(int seconds, SDL_Renderer *renderer, bool whites);

/**
 * @brief Frissíti az óra textúráját ha szükséges, és törli az előzőt
 * 
 * @param clock A frissítendő óra
 */
void updateClockTexture(Clock *clock);

/**
 * @brief Csökkenti az óra számlálóját, és elkészíti a textúrájt
 * 
 * @param clk Az óra
 * @return true Ha lejárt az óra
 * @return false Egyébként
 */
bool clockTick(Clock *clk);

/**
 * @brief Kirajzolja az órákat
 * @param board A tábla, aminek az óráit ki kell rajzolni
 */
void renderClocks(Board *board);

#endif