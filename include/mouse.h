/**
 * @file mouse.h
 * @brief Az egér állapotának kezelése
 */
#ifndef CHESS_MOUSE
#define CHESS_MOUSE

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "square.h"

typedef struct Board Board;

/**
 * @brief Az egér állapota
 */
typedef struct MouseState{
    bool held;      ///< Igaz, ha nyomva van tartva a gomb egy érvényes kiindulási négyzettől
    Square from;    ///< A nyomvatartás kiindulási mezője
    int xPos, yPos; ///< A jelenlegi x és y koordináták
} MouseState;

/**
 * @brief Ellenőrzi, hogy befejeződött a húzás
 * 
 * @param event Az event
 * @param ms Az egér állapota
 * @return true Ha vége a húzásnak
 * @return false Egyébként
 */
bool wasDragAndDrop(SDL_Event *event, MouseState ms);

/**
 * @brief Frissíti az egér állapotát (x, y, húzás)
 * 
 * @param event Az event
 * @param board A tábla
 * @param flip Meg kell e fordítani a táblát
 */
void updateMouseState(SDL_Event *event, Board *board, bool flip);

/**
 * @brief Az adott x,y koordinátát alakítja a sakktábla egy mezőjévé
 * 
 * @param x Az x koordináta
 * @param y Az y koordináta
 * @param flip Igaz, ha meg kell fordítani a táblát
 * @return Square Az adott mező
 */
Square mousePosToSquare(int x, int y, bool flip);
#endif