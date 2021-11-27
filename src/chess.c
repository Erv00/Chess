#include <stdio.h>
#include <assert.h>

#include "board.h"
#include "moves.h"
#include "algebraic.h"
#include "check.h"
#include "graphics.h"
#include "mouse.h"
#include "views.h"

#include "promotion.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "debugmalloc.h"

int main(void){
    //Initialization
    SDL_Window *window;
    SDL_Renderer *renderer;
    initGraphics(&window, &renderer);

    //Game loop
    while(!handleMenuView(renderer));
    
    //Free resources
    IMG_Quit();
    unloadPieces();
    unloadMenuTextures();
    unloadFonts();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
