#include "clock.h"
#include "graphics.h"

Clock newClock(int seconds, SDL_Renderer *renderer, bool whites){
    Clock ret = {
        .secondsRemaining = seconds,
        .texture = NULL,
        .renderer = renderer,
        .whitesClock = whites
        };
    updateClockTexture(&ret);
    return ret;
}

bool clockTick(Clock *clk){
    //Protect against underflow
    if(clk->secondsRemaining != 0)
        clk->secondsRemaining -= 1;

    updateClockTexture(clk);

    if(clk->secondsRemaining == 0)
        return true;
    
    return false;
}

void updateClockTexture(Clock *clock){
    //Free tex
    if(clock->texture != NULL)
        SDL_DestroyTexture(clock->texture);
    
    char str[6] = {0};
    sprintf(str, "%02d:%02d", clock->secondsRemaining/60, clock->secondsRemaining%60); //5 characters are enough
    
    //Render new texture
    clock->texture = winStringToTexture(clock->renderer, str, NULL, NULL, !clock->whitesClock);
}