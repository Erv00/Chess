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

//Updates the mouses x,y,held,form
bool wasDragAndDrop(SDL_Event *event, MouseState ms);

void updateMouseState(SDL_Event *event, Board *board, bool flip);

Square mousePosToSquare(int x, int y, bool flip);
#endif