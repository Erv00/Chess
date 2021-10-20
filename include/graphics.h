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

SDL_Texture *getPieceGraphics(Piece p);

void renderPiece(SDL_Renderer *renderer, Piece p, Square at);

void renderBoard(SDL_Renderer *renderer, bool flip);

void renderPieces(SDL_Renderer *renderer, Board *board);
#endif