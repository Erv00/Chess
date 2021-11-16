#ifndef CHESS_VIEWS
#define CHESS_VIEWS

#include <SDL2/SDL.h>

#include "board.h"

void renderMenuView(SDL_Renderer *renderer);
void renderNewGameView(SDL_Renderer *renderer);
void renderLoadView(SDL_Renderer *renderer);
void renderPlayView(Board *board);
void renderSaveView(SDL_Renderer *renderer, Board *board);
void renderGameOverView(Board *board);
void renderAnalysisView(Board *board);


void handleMenuView(SDL_Renderer *renderer);
void handleNewGameView(SDL_Renderer *renderer);
void handleLoadView(SDL_Renderer *renderer);
void handlePlayView(Board *board);
void handleSaveView(Board *board);
void handleGameOverView(Board *board);
void handleAnalysisView(Board *board);
#endif
