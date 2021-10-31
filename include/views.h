#ifndef CHESS_VIEWS
#define CHESS_VIEWS

#include <SDL2/SDL.h>

#include "board.h"

void renderMenuView(SDL_Renderer *renderer);
void renderNewGameView(SDL_Renderer *renderer);
void renderPlayView(Board *board);
void renderSaveView(SDL_Renderer *renderer, Board *board);
void renderGameOverView(SDL_Renderer *renderer, Board *board);
void renderAnalysisView(SDL_Renderer *renderer);


#endif