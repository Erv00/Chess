#ifndef CHESS_GRAPHICS
#define CHESS_GRAPHICS

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "piece.h"
#include "square.h"
#include "board.h"

typedef struct GraphicsData{
    SDL_Window *window;
    SDL_Renderer *renderer;
} GraphicsData;

//
GraphicsData initWindow(void);

bool loadPieces(SDL_Renderer *renderer);
void unloadPieces(void);

SDL_Texture *getPieceGraphics(Piece p);

void renderPiece(SDL_Renderer *renderer, Piece p, Square at);

void renderBoard(SDL_Renderer *renderer, bool flip);

void renderPieces(SDL_Renderer *renderer, Board *board);

//Creates a texture form a string. Stores it's width and height int with and height, any of these may be NULL
SDL_Texture* stringToTexture(SDL_Renderer *renderer, const char *str, int *width, int *height);
#endif
