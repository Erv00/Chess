#ifndef CHESS_BUTTONS
#define CHESS_BUTTONS

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef SDL_Rect Button;

//Waits until a button is pressed, returns its position
int waitForButtonPress(Button buttons[], int length);

//Checks if an event is a button press. If it is, the button's position is saved in buttonPressed, which must not be NULL
//Returnes true if a button was pressed, false otherwise
bool processEvent(Button buttons[], int length, SDL_Event *event, int *buttonPressed);
#endif
