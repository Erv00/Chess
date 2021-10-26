#ifndef CHESS_PROMOTION
#define CHESS_PROMOTION

#include <SDL2/SDL.h>
#include "board.h"
#include "square.h"
#include "piece.h"

//Draw the promotion picker
void drawPicker(Board *board);

//Returnes the piece choosen. Will return an invalid piece if cancelled
//Processes events, thus blocking main's event processing
Piece getChoice(Board *board);
Piece checkMouseSelect(Piece color, SDL_Event *e);
Piece checkKeyboardSelect(Piece color, SDL_Event *e);

#endif