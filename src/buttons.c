#include "buttons.h"

#include <stdio.h>
#include <stdlib.h>
#include <debugmalloc.h>

int waitForButtonPress(Button buttons[], int length){
    int pressed = -1;
    SDL_Event e;
    do{
        SDL_WaitEvent(&e);
    }while(!processEvent(buttons, length, &e, &pressed) && e.type != SDL_QUIT);

    return pressed;
}

bool processEvent(Button buttons[], int length, SDL_Event *event, int *buttonPressed){
    if(event->type != SDL_MOUSEBUTTONUP)
        return false;

    if(length == 0)
        return false;

    int x,y;
    SDL_GetMouseState(&x, &y);

    for(int i=0; i<length; i++){
        Button button = buttons[i];
        //X axis
        if(button.x < x && x < button.x+button.w)
            if(button.y < y && y < button.y + button.h){
                *buttonPressed = i;
                return true;
            }
    }
    return false;
}
