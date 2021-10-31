#include "views.h"

#include "graphics.h"
#include "analysis.h"

void renderMenuView(SDL_Renderer *renderer);
void renderNewGameView(SDL_Renderer *renderer);
void renderPlayView(Board *board){
    //Render board
    renderBoard(board->renderer, !board->nextIsWhite);

    //Render past steps
    renderReplay(board->replayData, board->renderer);

    //Render buttons
    SDL_Rect button = {
        .x = 8*45,
        .y = 7*45,
        .w = 4*45,
        .h = 45
    };

    SDL_SetRenderDrawColor(board->renderer, 0x76, 0x96, 0x56, 0xff);
    SDL_RenderFillRect(board->renderer, &button);
}
void renderSaveView(SDL_Renderer *renderer, Board *board);
void renderGameOverView(SDL_Renderer *renderer, Board *board);
void renderAnalysisView(SDL_Renderer *renderer);
