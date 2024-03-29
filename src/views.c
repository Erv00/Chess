#include "views.h"

#include "graphics.h"
#include "analysis.h"
#include "buttons.h"
#include "loading.h"

#include <assert.h>

#include <debugmalloc.h>
Uint32 oneSecondTick(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}
void renderMenuView(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, getMenuTexture(MENU_MAIN), NULL, NULL);
}
void renderNewGameView(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, getMenuTexture(MENU_NEW_GAME), NULL, NULL);
}
void renderLoadView(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer, getMenuTexture(MENU_LOAD), NULL, NULL);
}
void renderPlayView(Board *board){
    //Render board
    renderBoard(board->renderer, !board->nextIsWhite);

    //Render pieces
    renderPieces(board->renderer, board, !board->nextIsWhite);

    //Render past steps
    if(board->hasReplayData)
        renderReplay(board->replayData, board->renderer);

    //Render clock
    renderClocks(board);
}
void renderSaveView(SDL_Renderer *renderer){
    //Clear screen
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    //Show menu
    SDL_Texture *t = getMenuTexture(MENU_SAVE);
    SDL_Rect r = textureDim(t);
    r.x = (45*12-r.w)/2;
    r.y = (45*8-r.h)/2;
    SDL_RenderCopy(renderer, t, NULL, &r);
    
    SDL_RenderPresent(renderer);
}
void renderGameOverView(Board *board){
    SDL_RenderCopy(board->renderer, getMenuTexture(MENU_GAME_OVER), NULL, NULL);

    bool white = board->whiteWon;

    //Render extras
    Piece pieceToRender = QUEEN;
    SDL_Texture *winnerStr;
    SDL_Rect winnerDst = {.x = 4*45, .y = 45};

    SDL_Rect wonDst = {.x = 4*45, .y = 2*45};
    SDL_Texture *wonStr = winStringToTexture(board->renderer, "NYERT!", &wonDst.w, &wonDst.h, white);

    if(white){
        //White won
        pieceToRender |= COLOR_WHITE;
        winnerStr = winStringToTexture(board->renderer, "FEHÉR", &winnerDst.w, &winnerDst.h, white);
    }else{
        //Black won, also render white box
        SDL_Rect bg = {
            .x = 3*45,
            .y = 45,
            .w = 6*45,
            .h = 2*45
        };
        SDL_SetRenderDrawColor(board->renderer, 255,255,255,255);
        SDL_RenderFillRect(board->renderer, &bg);

        pieceToRender |= COLOR_BLACK;
        winnerStr = winStringToTexture(board->renderer, "FEKETE", &winnerDst.w, &winnerDst.h, white);
    }

    //Center texts
    winnerDst.x += (4*45-winnerDst.w)/2;
    wonDst.x += (4*45-wonDst.w)/2;

    //Render pieces
    SDL_Rect pieceDst = {.x = 3*45, .y = 2*45, .w = 45, .h = 45};
    SDL_RenderCopy(board->renderer, getPieceGraphics(pieceToRender), NULL, &pieceDst);
    pieceDst.x = 8*45;
    SDL_RenderCopy(board->renderer, getPieceGraphics(pieceToRender), NULL, &pieceDst);

    //Render texts
    SDL_RenderCopy(board->renderer, winnerStr, NULL, &winnerDst);
    SDL_RenderCopy(board->renderer, wonStr, NULL, &wonDst);

    //Free text textures
    SDL_DestroyTexture(wonStr);
    SDL_DestroyTexture(winnerStr);
}
void renderAnalysisView(Board *board){
    renderBoard(board->renderer, false);
    renderPieces(board->renderer, board, false);
    renderReplay(board->replayData, board->renderer);
    renderClocks(board);
}


bool handleMenuView(SDL_Renderer *renderer){
    Button buttons[] = {
        //New game
        {
            .x = 3*45,
            .y = 3*45,
            .w = 6*45,
            .h = 45
        },

        //Load
        {
            .x = 3*45,
            .y = 5*45,
            .w = 6*45,
            .h = 45
        },

        //Quit
        {
            .x = 9*45,
            .y = 7*45,
            .w = 3*45,
            .h = 45
        }
    };

    renderMenuView(renderer);
    SDL_RenderPresent(renderer);
    switch(waitForButtonPress(buttons, 3)){
        case 0: //New game
            return handleNewGameView(renderer);
        case 1: //Load
            return handleLoadView(renderer);
        default: //Quit
            return true;
    }

}
bool handleNewGameView(SDL_Renderer *renderer){
    Button buttons[] = {
        //15 min
        {
            .x = 2*45,
            .y = 2*45,
            .w = 2*45,
            .h = 2*45
        },

        //20 min
        {
            .x = 5*45,
            .y = 2*45,
            .w = 2*45,
            .h = 2*45
        },

        //25 min
        {
            .x = 8*45,
            .y = 2*45,
            .w = 2*45,
            .h = 2*45
        },

        //30 min
        {
            .x = 5*45,
            .y = 5*45,
            .w = 2*45,
            .h = 2*45
        },

        //Cancel
        {
            .x = 9*45,
            .y = 7*45,
            .w = 3*45,
            .h = 45
        }
    };

    renderNewGameView(renderer);
    SDL_RenderPresent(renderer);

    int pressed = waitForButtonPress(buttons, 5);
    if(pressed == -1)
        //User wants to quit
        return true;
    if(pressed == 4)
        //Cancel
        return false;
    
    int time = 15*60 + pressed*5*60;

    //Start game
    Board *board = newGameFromStart(renderer, time);
    handlePlayView(board);
    bool quit = board->quit;
    destroyBoard(board);
    return quit;
}
bool handleLoadView(SDL_Renderer *renderer){
    Button buttons[] = {
        //Continue
        {
            .x = 3*45,
            .y = 3*45,
            .w = 6*45,
            .h = 45
        },

        //Analysis
        {
            .x = 3*45,
            .y = 5*45,
            .w = 6*45,
            .h = 45
        },

        //Cancel
        {
            .x = 9*45,
            .y = 7*45,
            .w = 3*45,
            .h = 45
        }
    };

    renderLoadView(renderer);
    SDL_RenderPresent(renderer);
    Board *board = NULL;
    switch(waitForButtonPress(buttons, 3)){
        case 0: //Continue
            board = loadWithoutMoves(SAVE_PATH, renderer);
            if(board == NULL){
                //Not saved for cont
                return false;
            }
            board->renderer = renderer;
            handlePlayView(board);
            break;
        case 1: //Analysis
            board = loadMoves(SAVE_PATH, renderer);
            if(board == NULL){
                //Saced for cont
                return false;
            }
            board->renderer = renderer;
            handleAnalysisView(board);
            break;
        case -1: //Quit
            return true;
        //default: cancel
    }
    bool quit = false;
    if(board != NULL){
        quit = board->quit;
        destroyBoard(board);
    }
    return quit;
}
void handlePlayView(Board *board){
    //Save button
    SDL_Rect buttons[] = {{
        .x = 8*45,
        .y = 7*45,
        .w = 4*45,
        .h = 45
    }};

    SDL_Event ev;
    bool quit = false;
    int clk = SDL_AddTimer(1000, oneSecondTick, NULL);
    renderPlayView(board);
    SDL_RenderPresent(board->renderer);
    
    while(!quit && !board->quit && !board->gameOver && SDL_WaitEvent(&ev) != 0){
        //If pressed q, exit to main menu
        if(ev.type == SDL_KEYUP){
            if(ev.key.keysym.sym == SDLK_q){
                quit = true;
            }
        }else if(ev.type == SDL_QUIT){
            //User requested quit
            board->quit = true;
        }

        if(ev.type == SDL_USEREVENT){
            //Timer tick, decrease player time
            if(updateCorrectClock(board)){
                board->gameOver = true;
                board->draw = false;
                board->whiteWon = !board->nextIsWhite;
            }
        }

        int pressed;
        if(processEvent(buttons, 1, &ev, &pressed))
            //Pressed save button
            handleSaveView(board);

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

        //Render view
        renderPlayView(board);
        SDL_RenderPresent(board->renderer);
    }

    SDL_RemoveTimer(clk);

    if(board->gameOver && !board->draw)
        //Checkmate
        handleGameOverView(board);
}
void handleSaveView(Board *board){
    //Render menu
    renderSaveView(board->renderer);
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
            //Quit
            board->quit = true;
            return;
        case 0:
            //cancel
            return;
        case 1:
            //Save with path
            //Check if has path data
            if(board->hasReplayData)
                saveWithMoves(SAVE_PATH, board);
            else
                fprintf(stderr, "Board not loaded with path data, cannot save with it\n");
            
            break;
        case 2:
            //Normal save
            saveWithoutMoves(SAVE_PATH, board);
            break;
    }
}
void handleGameOverView(Board *board){
    //Buttons
    Button buttons[] = {
        //Main menu
        {
            .x = 3*45,
            .y = 5*45,
            .w = 2.5*45,
            .h = 45
        },

        //Save
        {
            .x = 6.5*45,
            .y = 5*45,
            .w = 2.5*45,
            .h = 45
        }
    };


    //Render end result
    renderGameOverView(board);
    SDL_RenderPresent(board->renderer);

    int buttonPressed = waitForButtonPress(buttons, 2);
    if(buttonPressed == -1){
        //Quit
        board->quit = true;
    }else if(buttonPressed == 1){
        //Clicked save button
        handleSaveView(board);
    }
}
void handleAnalysisView(Board *board){
    Button saveButton = {
        .x = 8*45,
        .y = 7*45,
        .w = 4*45,
        .h = 45
    };

    //Render view
    renderAnalysisView(board);
    SDL_RenderPresent(board->renderer);

    //Handle events
    SDL_Event ev;
    bool quit = false;
    while(!quit && SDL_WaitEvent(&ev) != 0){
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
                    freeAfterList(&board->replayData);

                    //Make move
                    movePieceWithCheck(board, m);

                    //Go to play view
                    handlePlayView(board);
                    return;
                }
            }
        }

        //Check if clicked on save button
        int clicked;
        if(processEvent(&saveButton, 1, &ev, &clicked)){
            //Pressed button
            handleSaveView(board);
        }

        //Update mouse and render
        updateMouseState(&ev, board, false);
        renderAnalysisView(board);
        SDL_RenderPresent(board->renderer);
    }
}
