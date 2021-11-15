#include "views.h"

#include "graphics.h"
#include "analysis.h"
#include "buttons.h"

#include <assert.h>

#include "debugmalloc.h"

void renderMenuView(SDL_Renderer *renderer);
void renderNewGameView(SDL_Renderer *renderer);
void renderPlayView(Board *board){
    //Render board
    renderBoard(board->renderer, !board->nextIsWhite);

    //Render pieces
    renderPieces(board->renderer, board, !board->nextIsWhite);

    //Render past steps
    renderReplay(board->replayData, board->renderer);
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
void renderAnalysisView(Board *board){
    renderBoard(board->renderer, false);
    renderReplay(board->replayData, board->renderer);
}


void handleMenuView(SDL_Renderer *renderer);
void handleNewGameView(SDL_Renderer *renderer);
void handlePlayView(Board *board){
    //Save button
    SDL_Rect buttons[1] = {{
        .x = 8*45,
        .y = 7*45,
        .w = 4*45,
        .h = 45
    }};

    SDL_Event ev;
    bool quit = false;
    while(!quit && SDL_WaitEvent(&ev) != 0){
        //Render view
        renderPlayView(board);
        SDL_RenderPresent(board->renderer);

        //If pressed q, exit to main menu
        if(ev.type == SDL_KEYUP){
            if(ev.key.keysym.sym == SDLK_q){
                quit = true;
            }
        }

        //If was dragging but dropped, make move
        if(wasDragAndDrop(&ev, board->mouseState)){
            Move m = {
                .from = board->mouseState.from,
                .to = mousePosToSquare(board->mouseState.xPos, board->mouseState.yPos, !board->nextIsWhite)
            };
            movePieceWithCheck(board, m);
        }

        //Update mouse state
        updateMouseState(&ev, board, !board->nextIsWhite);
    }
}
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
void handleAnalysisView(Board *board){
    //Render view
    renderAnalysisView(board);
    renderPieces(board->renderer, board, false);
    SDL_RenderPresent(board->renderer);

    //Handle events
    SDL_Event ev;
    bool quit = false;
    while(SDL_WaitEvent(&ev) != 0 && !quit){
        if(ev.type == SDL_QUIT)
            break;
        
        if(ev.type == SDL_KEYUP){
            quit = handleAnalysisKeypress(board, ev);
        }

        //Check for mouse events
        if(wasDragAndDrop(&ev, board->mouseState)){
            //Check if dnd was valid
            Move m = {
                .from = board->mouseState.from,
                .to = mousePosToSquare(board->mouseState.xPos, board->mouseState.yPos, false)
            };
            if(isValidSquare(m.to)){
                //D&D valid, check if move is valid
                if(isValidMove(board, m, NULL, NULL, NULL)){
                    //Drag&dropped a valid piece => start game from here
                    //Free moves after current move
                    freeListAfter(&board->replayData, board->replayData.last);

                    //Make move
                    movePieceWithCheck(board, m);
                    //return to previous view, which is play
                    quit = true;
                }
            }
        }

        //Update mouse and render
        updateMouseState(&ev, board, false);
        SDL_SetRenderDrawColor(board->renderer, 0,0,0,255);
        SDL_RenderClear(board->renderer);
        renderAnalysisView(board);
        renderPieces(board->renderer, board, false);
        SDL_RenderPresent(board->renderer);
    }
}
