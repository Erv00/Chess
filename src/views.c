#include "views.h"

#include "graphics.h"
#include "analysis.h"
#include "buttons.h"

#include <assert.h>

void renderMenuView(SDL_Renderer *renderer);
void renderNewGameView(SDL_Renderer *renderer);
void renderPlayView(Board *board){
    //Render board
    renderBoard(board->renderer, !board->nextIsWhite);

    //Render past steps
    renderReplay(board->replayData, board->renderer);

    //Render save button
    SDL_Rect button = {
        .x = 8*45,
        .y = 7*45,
        .w = 4*45,
        .h = 45
    };

    SDL_SetRenderDrawColor(board->renderer, 0x76, 0x96, 0x56, 0xff);
    SDL_RenderFillRect(board->renderer, &button);
}
void renderSaveView(SDL_Renderer *renderer, Board *board){
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    //Show menu
    SDL_Texture *t = getMenuTexture(MENU_SAVE);
    SDL_Rect r = textureDim(t);
    r.x = (45*12-r.w)/2;
    r.y = (45*8-r.h)/2;
    SDL_RenderCopy(renderer, t, NULL, &r);
    
    //Save path input: TODO
    SDL_RenderPresent(renderer);
}
void renderGameOverView(SDL_Renderer *renderer, Board *board);
void renderAnalysisView(SDL_Renderer *renderer);


void handleMenuView(SDL_Renderer *renderer);
void handleNewGameView(SDL_Renderer *renderer);
void handlePlayView(Board *board);
void handleSaveView(SDL_Renderer *renderer, Board *board){
    //Render menu
    renderSaveView(renderer, board);
    //Get save path: TODO
    char path[] = "./state.sch";
    //Check if saving step replay & save
    //Button positions
    Button cancel = {
        .x = 0 + 70,
        .y = 150-45 + 105,
        .w = 115,
        .h = 45
    };
    Button saveWithSteps = {
        .x = cancel.x + cancel.w + 20,
        .y = cancel.y,
        .w = 130,
        .h = 45
    };
    Button save = {
        .x = saveWithSteps.x + saveWithSteps.w + 20,
        .y = 150-45 + 105,
        .w = 115,
        .h = 45
    };

    Button buttons[] = {cancel, saveWithSteps, save};

    switch(waitForButtonPress(buttons, 3)){
        case -1:
        case 0:
            //cancel
            return;
        case 1:
            //Save with path
            saveWithMoves(path, board);
            break;
        case 2:
            //Normal save
            saveWithoutMoves(path, board);
            break;
    }
}
void handleGameOverView(SDL_Renderer *renderer, Board *board);
void handleAnalysisView(SDL_Renderer *renderer);
