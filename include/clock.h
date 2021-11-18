#ifndef CHESS_CLOCK
#define CHESS_CLOCK

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct Clock {
    int secondsRemaining;
    SDL_Texture *texture;
    bool whitesClock;

    SDL_Renderer *renderer;
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

#endif