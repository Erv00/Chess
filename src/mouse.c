#include "mouse.h"
#include "board.h"
#include "moves.h"
#include "views.h"
#include "debugmalloc.h"

bool wasDragAndDrop(SDL_Event *event, MouseState ms){
    if(event->type != SDL_MOUSEBUTTONUP)
        return false;
    
    if(ms.held && isValidSquare(ms.from))
        return true;
    return false;
}

void updateMouseState(SDL_Event *event, Board *board, bool flip){
    int x, y;
    SDL_GetMouseState(&x, &y);
    Square s = mousePosToSquare(x, y, flip);
    
    if((event->type & SDL_MOUSEMOTION) == SDL_MOUSEMOTION ){
        //Mouse event, handle
        //Update pos
        board->mouseState.xPos = x;
        board->mouseState.yPos = y;
        if(event->type == SDL_MOUSEBUTTONDOWN){
            //Press
            if(isValidSquare(s) && !isOpponentAt(board, s)){
                board->mouseState.held = true;
                board->mouseState.from = s;
            }
        } else if(event->type == SDL_MOUSEBUTTONUP){
            //Release
            board->mouseState.held = false;
        }
    }
}

Square mousePosToSquare(int x, int y, bool flip){
    Square res;
    if(!flip){
        res.rank = 7 - y / 45;
        res.file = x / 45;
    } else {
        res.rank = y / 45;
        res.file = 7 - x / 45;
    }

    return res;
}
