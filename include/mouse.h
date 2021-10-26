#ifndef CHESS_MOUSE
#define CHESS_MOUSE

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "square.h"

typedef struct Board Board;

typedef struct MouseState{
    bool held;
    Square from;
    int xPos, yPos;
} MouseState;

void handleMouseEvent(SDL_Event *event, Board *board);

Square mousePosToSquare(int x, int y, bool flip);
#endif