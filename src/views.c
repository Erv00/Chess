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
void renderAnalysisView(Board *board){
    renderBoard(board->renderer, false);
    renderReplay(board->replayData, board->renderer);
}


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
            switch(ev.key.keysym.sym){
                case SDLK_LEFT:
                    //Step back
                    if(board->replayData.length != 0){
                        //Undo step
                        undoMove(board, board->replayData.last);
                        board->replayData.last = board->replayData.last->previous;
                        board->replayData.length -= 1;
                        //Flip next turn
                        board->nextIsWhite = !board->nextIsWhite;
                    }
                    //DBG
                    printBoard(board);
                    break;
                case SDLK_RIGHT:
                    //Step forward
                    if(board->replayData.last == NULL){
                        //Step to start
                        //Make move
                        redoMove(board, board->replayData.first);
                        board->replayData.last = board->replayData.first;
                        board->replayData.length += 1;
                        //Flip next turn
                        board->nextIsWhite = !board->nextIsWhite;
                    } else if(board->replayData.last->next != NULL){
                        board->replayData.last = board->replayData.last->next;
                        redoMove(board, board->replayData.last);
                        board->replayData.length += 1;
                        //Flip next turn
                        board->nextIsWhite = !board->nextIsWhite;
                    }
                    //DBG
                    printBoard(board);
                    break;
                case SDLK_q:
                    quit = true;
                    break;
            }
        }
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
                    return;
                }
            }
        }
        updateMouseState(&ev, board, false);
        SDL_SetRenderDrawColor(board->renderer, 0,0,0,255);
        SDL_RenderClear(board->renderer);
        renderAnalysisView(board);
        renderPieces(board->renderer, board, false);
        SDL_RenderPresent(board->renderer);
    }
}
