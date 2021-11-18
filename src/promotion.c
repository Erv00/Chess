#include "promotion.h"
#include "graphics.h"
#include "debugmalloc.h"

void drawPicker(Board *board){
    Piece color = board->nextIsWhite ? COLOR_WHITE : COLOR_BLACK;
    static const Piece AVAILABLE_CHOICES[] = {ROOK, KNIGHT, BISHOP, QUEEN};
    //Select box
    SDL_Rect r = {
        .x = 2*45,
        .y = 3*45+45/2,
        .w = 4*45,
        .h = 45
    };
    SDL_SetRenderDrawColor(board->renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(board->renderer, &r);

    r.w = 45;
    r.h = 45;
    //Draw pieces
    for(int i = 0; i < 4; i++){
        SDL_RenderCopy(board->renderer, getPieceGraphics(AVAILABLE_CHOICES[i] | color), NULL, &r);
        r.x += 45;
    }
    SDL_RenderPresent(board->renderer);
}

Piece getChoice(Board *board){
    Piece color = board->nextIsWhite ? COLOR_WHITE : COLOR_BLACK;
    //Draw picker
    drawPicker(board);
    SDL_Event e;
    while(true){
        SDL_WaitEvent(&e);

        //Checks if event is a choice
        switch(e.type){
        case SDL_KEYDOWN:
            //Key event
            return checkKeyboardSelect(color, &e);
        case SDL_MOUSEBUTTONUP:
            //Mouse
            return checkMouseSelect(color, &e);
        case SDL_USEREVENT:
            //Tick clock
            updateCorrectClock(board);
            //TODO: Game over if clock runs out
            break;
        case SDL_QUIT:
            //TODO: HANDLE
            break;
        }
    }
}

Piece checkMouseSelect(Piece color, SDL_Event *e){
    int x,y;
    SDL_GetMouseState(&x, &y);
    if(y < 3.5*45 || y > 4.5*45){
        //Cancel
        return 0;
    }

    switch((x-2*45)/45){
    case 0:
        //Rook
        return color | ROOK;
    case 1:
        //Knight
        return color | KNIGHT;
    case 2:
        //Bishop
        return color | BISHOP;
    case 3:
        //Queen
        return color | QUEEN;
    default:
        //Cancel
        return 0;
    }
}

Piece checkKeyboardSelect(Piece color, SDL_Event *e){
    switch(e->key.keysym.sym){
    case SDLK_r:
        //Rook
        return color | ROOK;
    case SDLK_n:
        //Knight
        return color | KNIGHT;
    case SDLK_b:
        //Bishop
        return color | BISHOP;
    case SDLK_q:
        //Queen
        return color | QUEEN;
    default:
        //Cancel
        return 0;
    }
}