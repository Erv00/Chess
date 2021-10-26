#include "mouse.h"
#include "board.h"
#include "moves.h"

void handleMouseEvent(SDL_Event *event, Board *board){
    int x, y;
    SDL_GetMouseState(&x, &y);
    Square s = mousePosToSquare(x, y, !board->nextIsWhite);
    
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
                printf("Click at %d %d\n", s.rank, s.file);
            }
        } else if(event->type == SDL_MOUSEBUTTONUP){
            //Release
            board->mouseState.held = false;
            if(isValidSquare(s)){
                Move m = {
                    .from = board->mouseState.from,
                    .to = s
                };
                printf("Up at %d %d\n",s.rank,s.file);
                movePieceWithCheck(board, m);
                printBoard(board);
            }
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